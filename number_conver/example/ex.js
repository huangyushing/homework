var convert = require('../number_convert')
var num = '0'
var text = convert.num2text(num)
var num2 = convert.text2num(text)

console.log('num=%s => text=%s', num, text)
console.log('text=%s => num2=%s', text, num2)