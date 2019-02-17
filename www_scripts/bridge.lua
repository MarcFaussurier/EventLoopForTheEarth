--
-- Created by IntelliJ IDEA.
-- User: marcfsr
-- Date: 15/02/2019
-- Time: 16:35
-- To change this template use File | Settings | File Templates.
--

-- will append the www_scripts folder as a require path

function addIncludeDir(pathFromProjectRoot) -- path have to be without beggining/trailing slash
    package.path = package.path .. ';../' .. pathFromProjectRoot ..'/?.lua'
end

-- include required folders

addIncludeDir("www_scripts")
addIncludeDir("www_scripts/core")
addIncludeDir("www_scripts/controllers")
addIncludeDir("www_scripts/models")
addIncludeDir("www_scripts/views")

local main = require "main"

io.write("Lua script loaded\n")
io.write("test.hello content : " .. main.hello)

-- test function
function f (x, y)
    io.write("Inside F !!!")
    return (x^2 * math.sin(y))/(1 - x)
end

-- test function
function onRequest(filename, uri, cgiargs, buf, fd)


  main.routerInstance.match(filename)
  if uri == "/toto" then
      return "pomme de terre"
  else
      return ""
  end
end
