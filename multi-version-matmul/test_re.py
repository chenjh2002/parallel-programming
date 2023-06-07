import re

# 'const char r[9] = "\\\\\\\"", t[9] = "\n\\\"\\n\\\\\\";
string=re.match('\"(\\\\|\\\n|\\\"|[^\"])*\"', '"\n\\\"\\n\\\\\\";')
print(string)
