import math
import json
import random


class SpatialHash:
    def __init__(
        self,
        buckets=None,
        cell_width=None,
        cells_in_dimension=None,
        cell_offset_in_dimension=None,
    ):
        self.buckets = buckets
        self.cell_width = cell_width
        self.cells_in_dimension = cells_in_dimension
        self.cell_offset_in_dimension = cell_offset_in_dimension

    def gridToUnits(self, pos):
        return math.floor(pos - self.cell_offset_in_dimension) * self.cell_width

    def unitsToGrid(self, pos):
        return math.floor(pos / self.cell_width) + self.cell_offset_in_dimension

    def gridToUnitsVec(self, v):
        return v.clone().update(self.gridToUnits)

    def unitsToGridVec(self, v):
        return v.clone().update(self.unitsToGrid)

    # layout is rows then columns (row-major)
    # (x, y) is a cell pos, not unit pos
    def get_bucket_index(self, x, y):
        return y * self.cells_in_dimension + x

    def get_pos_for_bucket_index(self, bucket_index):
        y = math.floor(bucket_index / self.cells_in_dimension)
        x = bucket_index % self.cells_in_dimension
        return (x, y)

    def point_is_in_valid_range(self, point):
        gridCell = self.unitsToGridVec(point.clone())
        return (
            0 <= gridCell.x < self.cells_in_dimension
            and 0 <= gridCell.y < self.cells_in_dimension
        )

    def __repr__(self):
        return "<SpatialHash cell_width:%d cells_in_dimension:%d>" % (
            self.cell_width,
            self.cells_in_dimension,
        )


class Vec2d:
    def __init__(self, init=None, x=0, y=0):
        self.x = x
        self.y = y

        if init:
            self.x = init.x
            self.y = init.y

    def clone(self,):
        return Vec2d(init=self)

    def add(self, other):
        self.x += other.x
        self.y += other.y
        return self

    def sub(self, other):
        self.x -= other.x
        self.y -= other.y
        return self

    def mulScalar(self, scalar):
        self.x *= scalar
        self.y *= scalar
        return self

    def divScalar(self, scalar):
        self.x /= scalar
        self.y /= scalar
        return self

    def update(self, updater):
        self.x = updater(self.x)
        self.y = updater(self.y)
        return self

    def dot(self, v2):
        return self.x * v2.x + self.y * v2.y

    def equals(self, other):
        return self.x == other.x and self.y == other.y

    def __repr__(self):
        return "<Vec2d x:%f y:%f>" % (self.x, self.y)


def create_spatial_hash(triangles, cell_width, units_in_dimension=10000):
    print("create_spatial_hash")

    spatial_hash_cell_width = cell_width
    spatial_hash_units_in_dimension = units_in_dimension  # how wide is the whole area?
    spatial_hash_cells_in_dimension = math.ceil(
        spatial_hash_units_in_dimension / spatial_hash_cell_width
    )
    spatial_hash_buckets = (
        spatial_hash_cells_in_dimension * spatial_hash_cells_in_dimension
    )  # num buckets for equal size-length (square) grid
    spatial_hash_units_in_dimension = (
        spatial_hash_cells_in_dimension * spatial_hash_cell_width
    )
    spatial_hash_cell_offset_in_dimension = math.floor(
        spatial_hash_cells_in_dimension / 2
    )
    buckets = [None] * spatial_hash_buckets

    spatial_hash = SpatialHash(
        buckets=buckets,
        cell_width=spatial_hash_cell_width,
        cells_in_dimension=spatial_hash_cells_in_dimension,
        cell_offset_in_dimension=spatial_hash_cell_offset_in_dimension,
    )

    print("spatial_hash", spatial_hash)

    gridCellSz = spatial_hash_cell_width

    # https://www.mathopenref.com/coordtrianglearea.html
    def triangleArea(a, b, c):
        return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2

    def barycentric(p, a, b, c):
        # print("barycentric", p, a, b, c)
        v0 = b.clone().sub(a)
        v1 = c.clone().sub(a)
        v2 = p.clone().sub(a)
        d00 = v0.dot(v0)
        d01 = v0.dot(v1)
        d11 = v1.dot(v1)
        d20 = v2.dot(v0)
        d21 = v2.dot(v1)
        denom = d00 * d11 - d01 * d01
        v = (d11 * d20 - d01 * d21) / denom
        w = (d00 * d21 - d01 * d20) / denom
        u = 1.0 - v - w

        return (u, v, w)

    # takes grid coords
    def renderPixel(x, y, color):
        pass
        # ctx.fillStyle = color
        # ctx.fillRect(x * gridCellSz, y * gridCellSz, gridCellSz, gridCellSz)

    def pointRect(p, rx, ry, rw, rh):
        if (
            p.x >= rx
            and p.x <= rx + rw  # right of the left edge AND
            and p.y >= ry  # left of the right edge AND
            and p.y <= ry + rh  # below the top AND
        ):
            # above the bottom
            return True

        return False

    # LINE/RECTANGLE
    def lineRect(lineA, lineB, rx, ry, rw, rh):
        # check if the line has hit any of the rectangle's sides
        # uses the Line/Line function below
        left = lineLine(lineA.x, lineA.y, lineB.x, lineB.y, rx, ry, rx, ry + rh)
        right = lineLine(
            lineA.x, lineA.y, lineB.x, lineB.y, rx + rw, ry, rx + rw, ry + rh
        )
        top = lineLine(lineA.x, lineA.y, lineB.x, lineB.y, rx, ry, rx + rw, ry)
        bottom = lineLine(
            lineA.x, lineA.y, lineB.x, lineB.y, rx, ry + rh, rx + rw, ry + rh
        )

        # if ANY of the above are True, the line
        # has hit the rectangle
        if left or right or top or bottom:
            return True

        return False

    # http://www-cs.ccny.cuny.edu/~wolberg/capstone/intersection/Intersection%20point%20of%20two%20lines.html
    def lineLine(x1, y1, x2, y2, x3, y3, x4, y4):
        denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1)

        if denom == 0:
            # the two lines are parallel
            return False

        # calculate the direction of the lines
        uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / (denom)
        uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / (denom)

        # if uA and uB are between 0-1, line segments intersect
        if uA >= 0 and uA <= 1 and uB >= 0 and uB <= 1:
            return True

        return False

    def insertTriangle(tri, tri_index):
        a, b, c = tri
        # Compute triangle bounding box
        minX = spatial_hash.unitsToGrid(min(a.x, b.x, c.x))
        minY = spatial_hash.unitsToGrid(min(a.y, b.y, c.y))
        maxX = spatial_hash.unitsToGrid(max(a.x, b.x, c.x))
        maxY = spatial_hash.unitsToGrid(max(a.y, b.y, c.y))

        # Clip against grid bounds
        minX = max(minX, 0)
        minY = max(minY, 0)
        maxX = min(maxX, spatial_hash.unitsToGrid(spatial_hash_units_in_dimension))
        maxY = min(maxY, spatial_hash.unitsToGrid(spatial_hash_units_in_dimension))

        # Rasterize
        cellPos = Vec2d()
        for cellPosY in range(minY, maxY + 1):
            cellPos.y = cellPosY

            for cellPosX in range(minX, maxX + 1):
                cellPos.x = cellPosX

                triIntersectsCell = False
                cellCorners = []
                for offsetX in range(0, 2):
                    for offsetY in range(0, 2):
                        cellCorners.append(
                            spatial_hash.gridToUnitsVec(
                                cellPos.clone().add(Vec2d(x=offsetX, y=offsetY))
                            )
                        )

                # test cell corner inside triangle
                if triangleArea(a, b, c) == 0:
                    # triangle is degenerate, so we can't (and don't need to) check
                    # if a point lies inside it
                    pass
                else:
                    for corner in cellCorners:
                        # Determine barycentric coordinates
                        u, v, w = barycentric(corner, a, b, c)
                        # If p is on or inside all edges, render pixel.
                        if u >= 0 and v >= 0 and w >= 0:
                            triIntersectsCell = triIntersectsCell or True

                edges = [
                    [tri[0], tri[1]],
                    [tri[1], tri[2]],
                    [tri[2], tri[0]],
                ]

                # test cell edge intersects tri edge
                for edge in edges:
                    lineStart, lineEnd = edge
                    if lineRect(
                        lineStart,
                        lineEnd,
                        cellCorners[0].x,
                        cellCorners[0].y,
                        gridCellSz,
                        gridCellSz,
                    ):
                        triIntersectsCell = triIntersectsCell or True

                # test tri vert inside cell
                for vert in tri:
                    if pointRect(
                        vert, cellCorners[0].x, cellCorners[0].y, gridCellSz, gridCellSz
                    ):
                        triIntersectsCell = triIntersectsCell or True
                if triIntersectsCell:
                    # renderPixel(p.x, p.y, "rgba(255,0,0,1)")
                    bucket_index = spatial_hash.get_bucket_index(cellPos.x, cellPos.y)
                    if buckets[bucket_index] is None:
                        buckets[bucket_index] = []
                    buckets[bucket_index].append(tri_index)

    for game_tri_index, game_tri in enumerate(triangles):

        # only use x,z components of position for insertion into grid
        # this can create degenerate triangles (if they are aligned to the vertical axis)
        # so we need to handle that in the intersection testing code
        tri2d = [
            Vec2d(x=game_tri_vert[0], y=-game_tri_vert[2]) for game_tri_vert in game_tri
        ]
        for tri2d_vert in tri2d:
            assert spatial_hash.point_is_in_valid_range(
                tri2d_vert
            ), "triangle outside the spatial hash area: %s" % (tri2d_vert)

        insertTriangle(
            tri2d, game_tri_index,
        )

    return spatial_hash


def test(filepath):
    import turtle

    window_width = 1600
    window_height = 1200

    turtle.setup(window_width, window_height)
    wn = turtle.Screen()  # Get a reference to the window
    wn.title("spatial hash")  # Change the window title
    ctx = turtle.Turtle()  # Create our favorite turtle

    ctx.speed(0)
    turtle.tracer(0, 0)

    state = {"zoom": 0.25}

    def draw_line(start, end):
        ctx.penup()
        ctx.goto(start.x * state["zoom"], start.y * state["zoom"])
        ctx.pendown()
        ctx.goto(end.x * state["zoom"], end.y * state["zoom"])
        ctx.penup()

    with open(filepath) as json_file:
        triangles = json.load(json_file)
    # triangles = [triangles[64]]

    sh = create_spatial_hash(triangles, 120)
    bucket_sizes = [len(bucket) for bucket in sh.buckets if bucket is not None]
    print(sh)
    print(
        "minBucket",
        min(bucket_sizes),
        "maxBucket",
        max(bucket_sizes),
        "avgBucket",
        sum(bucket_sizes) / len(bucket_sizes),
    )

    def vert_to_vec2d(vert):
        # x,-z to x,y
        return Vec2d(x=vert[0], y=-vert[2])

    def draw_triangle(tri):
        edges = [
            [tri[0], tri[1]],
            [tri[1], tri[2]],
            [tri[2], tri[0]],
        ]
        for edge in edges:
            draw_line(vert_to_vec2d(edge[0]), vert_to_vec2d(edge[1]))

    def draw_triangles():
        for tri in triangles:
            draw_triangle(tri)

    def randcolor():
        return random.randint(0, 255)

    def rectangle(x, y, width, height):
        ctx.penup()
        ctx.begin_fill()
        ctx.goto(x * state["zoom"], y * state["zoom"])
        ctx.pendown()
        ctx.goto((x + width) * state["zoom"], y * state["zoom"])
        ctx.goto((x + width) * state["zoom"], (y + height) * state["zoom"])
        ctx.goto(x * state["zoom"], (y + height) * state["zoom"])
        ctx.goto(x * state["zoom"], y * state["zoom"])
        ctx.end_fill()
        ctx.penup()

    def highlight_bucket(x, y):
        render()

        print("highlight_bucket", x, y)
        gridX = int(sh.unitsToGrid(x / state["zoom"]))
        gridY = int(sh.unitsToGrid(y / state["zoom"]))
        print("gridX, gridY", gridX, gridY)
        bucket_index = sh.get_bucket_index(gridX, gridY)
        print("bucket_index", bucket_index)
        bucket = sh.buckets[bucket_index]

        ctx.fillcolor("blue")
        ctx.pensize(3)

        rectangle(
            sh.gridToUnits(gridX), sh.gridToUnits(gridY), sh.cell_width, sh.cell_width,
        )

        turtle.colormode(255)
        if bucket:
            for tri_index in bucket:
                ctx.pencolor((randcolor(), randcolor(), randcolor()))
                draw_triangle(triangles[tri_index])
            ctx.pencolor("black")
            for tri_index in bucket:
                draw_label(triangles[tri_index], tri_index)

        ctx.goto(window_width / 2 - 130, window_height / 2 - 30)
        style = ("Courier", 20)
        ctx.write("x=%d y=%d" % (gridX, gridY), font=style, align="center")
        turtle.update()

    def draw_grid():
        half_width = window_width / 2
        half_height = window_height / 2

        top = -half_height / state["zoom"]
        bottom = half_height / state["zoom"]
        left = -half_width / state["zoom"]
        right = half_width / state["zoom"]

        for gridX in range(sh.unitsToGrid(left), sh.unitsToGrid(right),):
            draw_line(
                Vec2d(x=sh.gridToUnits(gridX), y=top),
                Vec2d(x=sh.gridToUnits(gridX), y=bottom),
            )
        for gridY in range(sh.unitsToGrid(top), sh.unitsToGrid(bottom),):
            draw_line(
                Vec2d(y=sh.gridToUnits(gridY), x=left),
                Vec2d(y=sh.gridToUnits(gridY), x=right),
            )

    def triangle_center(tri):
        return tri[0].add(tri[1]).add(tri[2]).divScalar(3)

    def draw_label(tri, tri_index):
        center = triangle_center([vert_to_vec2d(vert) for vert in tri])
        ctx.penup()
        ctx.goto(center.x * state["zoom"], center.y * state["zoom"])
        style = ("Lucida Grande", 11)
        ctx.write(str(tri_index), font=style, align="center")

    def zoomIn():
        state["zoom"] *= 2
        render()

    def zoomOut():
        state["zoom"] /= 2
        render()

    def render():
        ctx.clear()
        ctx.pensize(1)
        ctx.pencolor("lightgrey")
        draw_grid()
        ctx.pencolor("black")
        draw_triangles()

    turtle.update()

    render()

    wn.listen()

    wn.onclick(highlight_bucket)
    wn.onkey(zoomIn, "e")
    wn.onkey(zoomOut, "q")

    wn.mainloop()  # This will make sure the program continues to run


if __name__ == "__main__":
    test("university_map_collision.json")
