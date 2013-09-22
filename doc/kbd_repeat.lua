-- kbd_repeat
-- @short: Change keyboard repeat rate 
-- @inargs: repeatrate 
-- @longdescr: Keyboard- and mouse input devices are treated as somewhat special
-- compared to other forms of input, and therefore has more global states tracked.
-- This function is used to enable (rrate > 0) or disable (rrate == 0) emitting
-- "push/release" input events for keyboard devices.
-- @group: iodev 
-- @cfunction: arcan_lua_kbdrepeat
-- @planned: The long-term plan for this part of the input system is
-- to be able to specify a repeat-rate on a device:(optional subid) bases
-- for all digital data-sources, not just the active keyboard.
