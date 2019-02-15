-- test object
local function HomeController(router)
  -- pubic variables go here
  local self = {
    -- routes["SOMEWHAT"] -> route, controller, method
    routes = {}
  }

  -- private variables go here
  local private = {
    router = null
  }

  function self.homePage()

  end
  router.registerRoute("/to*", self, "homePage")

  function constructor(router)
      private.router = router
      io.write("home controller built")
  end
  -- calling the constructor and returning instance
  constructor(router); return self
end

return HomeController
