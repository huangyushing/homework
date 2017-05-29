var num2text = require('./num2text.js')
var text2num = require('./text2num.js')

var M = module.exports = {}

M.charMap = function (s, s2t) {
  var t = s2t[s]
  return (typeof t === 'undefined') ? s : t
}

M.textMap = function (text, s2t) {
  var toText = []
  for (var i = 0; i < text.length; i++) {
    toText[i] = M.charMap(text[i], s2t)
  }
  return toText.join('')
}

M.text2num = function (text) {
  return M.textMap(text, text2num)
}

M.num2text = function (text) {
  return M.textMap(text, num2text)
}
