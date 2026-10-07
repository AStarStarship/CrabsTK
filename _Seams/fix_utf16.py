import os
for path in ['../Pro/_Config.hxx', '../Touch/_Config.hxx', '../Who/_Config.hxx']:
    if os.path.exists(path):
        with open(path, 'rb') as f:
            data = f.read()
        if data.startswith(b'\xff\xfe'):
            data = data.decode('utf-16le').encode('utf-8')
            with open(path, 'wb') as f:
                f.write(data)
