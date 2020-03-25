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

c_files = glob.glob("*.c")
full_rebuild_deps = glob.glob("*.h") + glob.glob("spec") + glob.glob("Makefile")

c_files_mtimes = {filepath: os.path.getmtime(filepath) for filepath in c_files}
full_rebuild_deps_mtimes = {
    filepath: os.path.getmtime(filepath) for filepath in full_rebuild_deps
}


changed_files = []

# if any c file is change, bust the cache for that file
for file, mtime in c_files_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        changed_files.append(file)

# if any of these files are changed, just bust the whole build cache
for file, mtime in full_rebuild_deps_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        changed_files = c_files

if len(changed_files) > 0:
    print(" ".join(changed_files))

updated_cache = {**c_files_mtimes, **full_rebuild_deps_mtimes}
with open(CACHE_FILE, "w") as outfile:
    json.dump(updated_cache, outfile)
