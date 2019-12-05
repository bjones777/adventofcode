let total = 0;
let total2 = 0;
let digits = [];
const maxIndex = 6;
const low = 265275;
const high = 781584;

function hasDouble() : boolean {
  for(let i = 1;i < maxIndex;++i) {
    if(digits[i-1] === digits[i]) {
      return true;
    }
  }
  return false;
}

function isValid() : boolean {
  for(let i = 1;i < maxIndex;++i) {
    if(digits[i-1] > digits[i]) {
      return false;
    }
  }
  let num = digits.reduce((p,c) => p * 10 + c,0);
  if(num < low || num > high) {
    return false;
  }
  return hasDouble();
}

function isValid2() : boolean {
  for(let d = 0;d < 10;++d) {
    let run = 0;
    for(let i = 0;i < maxIndex;++i) {
      if(digits[i] === d) {
        run += 1;
      }
      else {
        if(run === 2) {
          return true;
        }
        run = 0;
      }
    }
    if(run === 2) {
      return true;
    }
  }
  return false;
}

function recusive(index) {
  if(index >= maxIndex) {
    if (isValid()) {
      total += 1;
      if(isValid2()) {
        total2 += 1;
      }
    }
    return;
  }
  for(let i = 0;i < 10;++i) {
    digits[index] = i;
    recusive(index+1);
  }
}

recusive(0);
console.log(total);
console.log(total2);