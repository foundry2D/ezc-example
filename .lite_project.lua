local core = require "core"
local command = require "core.command"
local keymap = require "core.keymap"
local console = require "plugins.console"

command.add(nil, {
	["kinc:build-project"] = function()
		core.log "Building..."
		console.run {
			command = "node Kinc/make  ",
			file_pattern = "(.*):(%d+):(%d+): (.*)$",
			on_complete = function() core.reschedule_project_scan() core.log "Build complete" end,
		}
	end,
	["kinc:compile-project"] = function()
		core.log "Building..."
		console.run {
			command = "node Kinc/make --compile ",
			file_pattern = "(.*):(%d+):(%d+): (.*)$",
			on_complete = function() core.reschedule_project_scan() core.log "Build complete" end,
		}
	end,
})

keymap.add { ["ctrl+b"] = "kinc:build-project",["ctrl+shift+b"] = "kinc:compile-project" }
