with open("../Image/SOIL2.hxx", "r") as f:
    content = f.read()

import re
content = content.replace("      (void*)glXGetProcAddress", "      glXGetProcAddress")
content = content.replace("      (void*)((const GLubyte *)proc);", "      ((const GLubyte *)proc);")

# We want to change func = ... to func = (void*)(...)
content = re.sub(r'func =\n#if !defined\(GLX_VERSION_1_4\)\n\s*glXGetProcAddressARB\n#else\n\s*glXGetProcAddress\n#endif\n\s*\(\(const GLubyte \*\)proc\);', 
                 'func = (void*)(\n#if !defined(GLX_VERSION_1_4)\n      glXGetProcAddressARB\n#else\n      glXGetProcAddress\n#endif\n      ((const GLubyte *)proc));', content)

with open("../Image/SOIL2.hxx", "w") as f:
    f.write(content)
