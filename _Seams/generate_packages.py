import os
import glob

modules = ["Audio", "Code", "Data", "Forms", "GUI", "Image", "IMUL", "Pro", "Touch", "Who"]

root_package = "// Copyright AStarship <https://astarship.net>.\n"

for mod in modules:
    if not os.path.isdir(f"../{mod}"):
        continue
    
    hxx_files = glob.glob(f"../{mod}/*.hxx")
    hxx_files = [os.path.basename(f) for f in hxx_files if not f.endswith("_Package.hxx")]
    
    if not hxx_files:
        continue
        
    pkg_content = "// Copyright AStarship <https://astarship.net>.\n"
    for hxx in sorted(hxx_files):
        pkg_content += f'#include "{hxx}"\n'
        
    with open(f"../{mod}/_Package.hxx", "w") as f:
        f.write(pkg_content)
        
    root_package += f'#include "{mod}/_Package.hxx"\n'

with open("../_Package.hxx", "w") as f:
    f.write(root_package)

print("Generated _Package.hxx files")
