import os, glob

for root, dirs, files in os.walk("../Image"):
    for file in files:
        if not file.endswith((".h", ".hxx")): continue
        path = os.path.join(root, file)
        with open(path, "r") as f:
            content = f.read()
        
        # We need to replace:
        # errno_t err = fopen_s(&f, filename, "rb");
        # if (!err) return 0;
        # with:
        # f = fopen(filename, "rb");
        # if (!f) return 0;
        import re
        
        # match: errno_t err = fopen_s(&f, filename, "rb"); \n if (!err) return 0;
        def repl(m):
            f_var = m.group(1)
            filename_var = m.group(2)
            mode = m.group(3)
            return f"{f_var} = fopen({filename_var}, \"{mode}\");\n  if (!{f_var})"
        
        new_content = re.sub(r'errno_t\s+err\s*=\s*fopen_s\(&([a-zA-Z0-9_]+),\s*([^,]+),\s*"([^"]+)"\);\s*\n\s*if\s*\(!err\)', repl, content)
        
        # For stbi_test and others, we might have `if (!err) return STBI_unknown;` etc.
        # Wait, the regex `if \(!err\)` catches it and the rest of the line is unchanged!
        # Wait! "if (!err) {" also exists.
        def repl2(m):
            f_var = m.group(1)
            filename_var = m.group(2)
            mode = m.group(3)
            return f"{f_var} = fopen({filename_var}, \"{mode}\");\n  if (!{f_var})"

        new_content = re.sub(r'errno_t\s+err\s*=\s*fopen_s\(&([a-zA-Z0-9_]+),\s*([^,]+),\s*"([^"]+)"\);\s*\n\s*if\s*\(!err\)', repl2, content)
        
        if new_content != content:
            with open(path, "w") as f:
                f.write(new_content)

