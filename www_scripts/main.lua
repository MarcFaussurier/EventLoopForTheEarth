--
-- Created by IntelliJ IDEA.
-- User: marcfsr
-- Date: 15/02/2019
-- Time: 16:25
-- To change this template use File | Settings | File Templates.
--
local Router = require "core.router"
local HomeController = require "controllers.home"

m = {}
m.routerInstance = Router()
m.HomeControllerInstance = HomeController(m.routerInstance)
m.hello = "world"

return m
