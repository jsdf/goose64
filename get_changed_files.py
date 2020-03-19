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
h_files = glob.glob("*.h")


c_files_mtimes = {filepath: os.path.getmtime(filepath) for filepath in c_files}
h_files_mtimes = {filepath: os.path.getmtime(filepath) for filepath in h_files}


changed_files = []

# if any c file is change, bust the cache for that file
for file, mtime in c_files_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        changed_files.append(file)

# if any h file is changed, just bust the whole build cache
for file, mtime in h_files_mtimes.items():
    if previous_mtimes.get(file, 0) < mtime:
        changed_files = c_files

if len(changed_files) > 0:
    print(" ".join(changed_files))

updated_cache = {**c_files_mtimes, **h_files_mtimes}
with open(CACHE_FILE, "w") as outfile:
    json.dump(updated_cache, outfile)
