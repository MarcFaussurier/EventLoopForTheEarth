io.write("Lua script loaded")
function f (x, y)
    io.write("Inside F !!!")
    return (x^2 * math.sin(y))/(1 - x)
end

function onRequest(filename, uri, cgiargs, buf, fd)
    io.write("got REQUEST !!! ")
    io.write(uri);
    if uri == "/toto" then
       return "pomme de terre"
    else
        return ""
    end
end
