local table = {}

--[[
Adds a configuration table to the top of the stack in the host

key types can be: { string }
 string key types are allowed
  table[a]   = 'valid'
  table['1'] = 'valid'

 integer key types are an error
  table[100] = 'invalid'

value types can be: { string, number }
 string value types are allowed
  table[a] = 'hello world'
 number value types are allowed
  table[a] = 3.14159
--]]

table.key1  = 'a'
table.key2  = 'b'
table.key3  = 10.33
table['1'] = 'notice me'


print('\navailable functions:')
local i = nil;
repeat
  i,v = next(_G,i)
  if type(v) == "function" and i ~= 'v' then
    print('',i)
  end
until not i

print('')

-- 'table' gets added to the stack
return table
