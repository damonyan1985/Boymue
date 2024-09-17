var require = boymue.require;

// require('/example/import.js');
// require('/example/import.js');
// require('/example/import.js');

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

function parseDom(xml) {
    return new Promise((resolve, reject) => {
        boymue.xmlToJson('/example/test.xml', (res) => {
            boymue.log('json: ' + res)
            let xmlObj = JSON.parse(res)

            boymue.log('count: ' + xmlObj.count)
            resolve(xmlObj)
        })
    })
}

function request(obj) {
    return new Promise((resolve, reject) => {
        boymue.request(JSON.stringify(obj), (res) => {
            boymue.log('request function result: ' + res + ', resolve:' + resolve)
            resolve(res)
        })
    })
}

//boymue.request(JSON.stringify({
//    'method': 'GET',
//    'url': 'https://127.0.0.1:8443/user/v1/testlogin',
//    'headers': {'token': 'none'}
//}), (res) => {
//    boymue.log('boymue.request result: ' + res)
//})

// async function test() {
//     let data = await request({
//         'method': 'GET',
//         'url': 'https://127.0.0.1:8443/user/v1/testlogin',
//         'headers': {'token': 'none'}
//     });

//     boymue.log('test async request: ' + data);

//     return data;
// }



 async function testParseXml() {
     let obj = await parseDom('/example/test.xml')
//     let vdom = new VDom(obj);
     boymue.log('testParseXml: ' + JSON.stringify(obj))
     //boymue.log('testParseXml: ')
}

class P {
    //get = () => "123"
}

// test()
testParseXml()

require('/example/vdom.js')


//boymue.log(testimport());

boymue.gc();

//boymue.log("test get=" + (new P().get()))