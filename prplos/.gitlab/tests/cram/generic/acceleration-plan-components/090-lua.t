Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that lua-amx module is behaving as expected:

  $ R "cat > /tmp/cram_test.lua <<EOF
  > local lamx = require('lamx')
  > lamx.dm.create()
  > lamx.config.set('Foo.Hello', 'Lua!')
  > lamx.config.set('Foo.Quiz', { Answer = 42 })
  > print('Hello ' .. lamx.config.get('Foo').Hello)
  > print('Answer is ' .. lamx.config.get('Foo').Quiz.Answer)
  > lamx.dm.destroy()
  > EOF
  > lua /tmp/cram_test.lua ; rm /tmp/cram_test.lua"
  Hello Lua!
  Answer is 42

Check that mod-lua-amx is behaving as expected:

  $ R "mkdir -p /etc/amx/luatest && ln -s /usr/bin/amxrt /usr/bin/luatest &&
  > cat > /etc/amx/luatest/luatest.odl <<EOF
  > import 'mod-lua-amx.so' as 'mod_lua';
  > %define { entry-point mod_lua.mod_lua_main; }
  > %define {
  >     object X_LuaTest {
  >             string greet(%in %mandatory string who) <!LUA:
  >                     function(object, args) return 'Hello ' .. args.who end
  >             !>;
  >     }
  > }
  > EOF
  > luatest -D ; ubus -t2 wait_for X_LuaTest ; ubus -S call X_LuaTest greet '{\"who\":\"World\"}' ;
  > pkill -9 luatest; rm -fr /etc/amx/luatest /usr/bin/luatest"
  {"retval":"Hello World"}
  {}
  {"amxd-error-code":0}
