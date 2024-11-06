# POC_OpenGL_To_WebGL

## Generate the solution (.sln)

1. start the file "intall.bat"

## Generate to web

> :warning: If you want generate the c++ solution on the web, you need install emscripten manualy

1. create and join a new repository

   > mkdir web && cd web

2. compile the solution to wasm, js and html file

   > emcmake cmake ..

3. start on web server, index.html (test page)
