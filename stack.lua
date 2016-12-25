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

-- 'table' gets added to the stack
return table
