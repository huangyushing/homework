var assert = require('assert')
var convert = require('../number_convert')

describe('convert', function () {
  it('test num2text', function () {
    assert.equal(convert.num2text('0'), '零')
  })
  it('test text2num', function () {
    assert.equal(convert.text2num('零'), '0')
  })
})
