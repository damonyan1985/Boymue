function test(a, b) {
  let arr = [0, 1, 2];
  arr.push(5);
  boymue.log('test arr length=' + arr.length);
  return a * b * arr[2];
}

boymue.log(test(2, 3));