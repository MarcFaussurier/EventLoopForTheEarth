-- test object
local function Router()
  -- pubic variables go here
  local self = {
    -- routes["SOMEWHAT"] -> route, controller, method
    routes = {}
  }

  -- private variables go here
  local private = {
    init = 0
  }

  function self.registerRoute(route, controller, method)
    table.insert(self.routes, {
       route = route,
       controller = controller,
       method = method
    })
    io.write("ROUTE REGISTRED!!!!")
  end

  function self.match(url)
    io.write("Marching url ... " .. url)
    for k,v in pairs(self.routes) do
      io.write(k .. " " .. v["route"]);
      local t = {}                   -- table to store the indices
      local i = 0
      while true do
        i = string.find(url, v["route"], i+1)    -- find 'next' newline
        if i == nil then break end
        table.insert(t, i)
      end
      if not (next(table) == nil) then
        io.write(v["route"] .. " MATCHED URL => " .. url)
      end
      -- body...
    end
  end

  function constructor()
    io.write("router built")
  end
  -- calling the constructor and returning instance
  constructor(params); return self
end

return Router
