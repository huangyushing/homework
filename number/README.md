

##Introduction

```
npm install number_to_convert
```

##Use number_convert
file : ex.js
```javascript
var convert = require('../number_convert')
var num = '0'
var text = convert.num2text(num)
var num2 = convert.text2num(text)

console.log('num=%s => text=%s', num, text)
console.log('text=%s => num2=%s', text, num2)
```
##Run
```
$ node ex.js
num=0 => text=零
text=零 => num2=0
 ```
##TO DO
 ```
 大寫中文數字轉換
 ```