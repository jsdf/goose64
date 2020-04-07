import json
import glob
import os

CACHE_FILE = "./buildcache.json"


previous_mtimes = {}


try:
    with open(CACHE_FILE) as fp:
        previous_mtimes = json.load(fp)
except FileNotFoundError:
    pass


def get_mtimes_map(filelist):
    return {filepath: os.path.getmtime(filepath) for filepath in filelist}


c_files = glob.glob("*.c")

obj_model_files = glob.glob("*.obj")
model_headers = set([obj[:-3] + "h" for obj in obj_model_files])

full_rebuild_deps = glob.glob("Makefile") + [
    header for header in glob.glob("*.h") if header not in model_headers
]
link_deps = glob.glob("spec") + glob.glob("sounds/*.*")

c_files_mtimes = get_mtimes_map(c_files)
full_rebuild_deps_mtimes = get_mtimes_map(full_rebuild_deps)
link_deps_mtimes = get_mtimes_map(link_deps)
model_deps_mtimes = get_mtimes_map(model_headers)


invalid_files = []


def get_artifact_for_c_file(filepath):
    return filepath[:-1] + "o"


def get_artifacts_for_spec_file():
    return glob.glob("*.out") + glob.glob("*.n64")


# if any c file changed, bust the relevant .o file
for file, mtime in c_files_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        invalid_files.append(get_artifact_for_c_file(file))

# if Makefile or a header file (with some exceptions) changed, bust everything
for file, mtime in full_rebuild_deps_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        invalid_files += [get_artifact_for_c_file(file) for file in c_files]

# spec file: only bust final output
for file, mtime in link_deps_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        invalid_files += get_artifacts_for_spec_file()

# model headers: only bust models segment
for file, mtime in model_deps_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        invalid_files += [
            get_artifact_for_c_file(file) for file in glob.glob("models.c")
        ]

invalid_files = list(set(invalid_files))

if len(invalid_files) > 0:
    print(" ".join(invalid_files))

updated_cache = {
    **c_files_mtimes,
    **full_rebuild_deps_mtimes,
    **link_deps_mtimes,
    **model_deps_mtimes,
}
with open(CACHE_FILE, "w") as outfile:
    json.dump(updated_cache, outfile)
