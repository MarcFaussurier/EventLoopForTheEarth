io.write("Lua script loaded")
function f (x, y)
    io.write("Inside F !!!")
    return (x^2 * math.sin(y))/(1 - x)
end

function onRequest()

end