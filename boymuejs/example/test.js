function testFunc(a, b) {
  let arr = [0, 1, 2];
  arr.push(5);
  boymue.log('test arr length=' + arr.length);
  boymue.test("only test", (res) => {
    boymue.log('only test result: ' + res);
  });
  return a * b * arr[2];
}

class App {
  static hello() {
    return 'hello app'
  }
}

boymue.log(testFunc(2, 3));

boymue.log(App.hello());

boymue.xmlToJson('/example/test.xml', (res) => {
    boymue.log('json: ' + res);
})




