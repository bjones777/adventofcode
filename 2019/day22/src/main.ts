import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

  const numCards = 10007;
  //const numCards = 10;

  let deck = new Array(numCards).fill(0).map((v,i) => i);

  function newDeal() {
    deck = new Array(numCards).fill(0).map((v,i) => i);
  }

  function dealIntoNewStack() {
    deck = deck.reverse();
  }

  function cut(x : number) {
    if(x > 0) {
      const top = deck.slice(0,x);
      const bottom = deck.slice(x);
      deck = bottom.concat(top);
    }
    else {
      const top = deck.slice(deck.length+x);
      const bottom = deck.slice(0,deck.length+x);
      deck = top.concat(bottom);
    }
  }

  function dealWithIncrement(i : number) {
    const newDeck = new Array(deck.length);
    let di = 0;
    for(let si = 0;si < deck.length;++si) {
      newDeck[di] = deck[si];
      di += i;
      while(di >= deck.length) {
        di -= deck.length;
      }
    }
    deck = newDeck;
  }

  function test1() {
    newDeal();
    dealWithIncrement(7);
    dealIntoNewStack();
    dealIntoNewStack();
    
    const expected = [0,3,6,9,2,5,8,1,4,7];
    for(let i = 0;i < expected.length;++i) {
      if(expected[i] != deck[i]) {
        throw new Error(`${deck} ${expected}`);
      }
    }
  }

  function test2() {
    newDeal();
    cut(6);
    dealWithIncrement(7);
    dealIntoNewStack();
    const expected = [3,0,7,4,1,8,5,2,9,6];
    for(let i = 0;i < expected.length;++i) {
      if(expected[i] != deck[i]) {
        throw new Error(`${deck} ${expected}`);
      }
    }
  }

  function test3() {
    newDeal();
    dealWithIncrement(7);
    dealWithIncrement(9);
    cut(-2);
    const expected = [6,3,0,7,4,1,8,5,2,9];
    for(let i = 0;i < expected.length;++i) {
      if(expected[i] != deck[i]) {
        throw new Error(`${deck} ${expected}`);
      }
    }
  }

  const lines = [];

  function runTests() {
    test1();
    test2();
    test3();
  }

  //runTests();

  
  function doLines() {
    lines.forEach(line => {
      if(line.startsWith("deal into new stack")) {
        dealIntoNewStack();
      }
      else if(line.startsWith("cut ")) {
        cut(Number(line.substr(4)));
      }
      else if(line.startsWith("deal with increment ")) {
        dealWithIncrement(Number(line.substr(20)));
      }
      else {
        throw new Error(`Unknown line: ${line}`);
      }
    });
  }

rl.on('line',(line : string) => {
    lines.push(line);
    
}); 

class lcf {
  private a : bigint;
  private b : bigint;

  constructor(a : bigint, b: bigint) {
    this.a = a;
    this.b = b;
  }

  compose(inner : lcf, m : bigint) : lcf {
    let c = (this.a * inner.a) % m;
    let d = (this.a * inner.b + this.b) % m;
    return new lcf(c,d);
  }

  evaluate(x : bigint, m : bigint) : bigint {
    return (this.a * x + this.b) % m;
  }

  print() {
    console.log(`f(n) = ${this.a} * x + ${this.b}`);
  }

  solve(d : bigint, m: bigint) : bigint[] {
    let b = d - this.b;
    let res = extendedEuclid(this.a, m);
    if(res[0] != 0n || b != 0n) {
      let arr = [];
      let x0 = (res[1] * (b/res[0])) % m;
      for(let i = 0n;i <= res[0];++i) {
        arr.push((x0 + i*m/res[0]) % m);
      }
      return arr;
    }
    return [];
  }
}

function pow_compose(fn : lcf, e : bigint, m : bigint) : lcf {
  if(e === 1n) {
    return fn;
  }
  let fn2 = pow_compose(fn,e/2n,m);
  fn2 = fn2.compose(fn2,m);
  if(e % 2n === 1n) {
    fn2 = fn.compose(fn2,m);
  }
  return fn2;
}

function pow_compose2(fn : lcf, e : bigint, m : bigint) : lcf {
  let g = new lcf(1n, 0n);
    while (e > 0) {
        if (e % 2n === 1n) {
          g = g.compose(fn,m);
        }
        e = e/2n;
        fn = fn.compose(fn,m);
    }
    return g;
}

function extendedEuclid(a : bigint, b: bigint) : bigint[] {
  let rst = [b, 0n, 1n];
  let rsto = [a, 1n, 0n];
  while(rst[0] != 0n) {
    let q = rsto[0] / rst[0];
    
    [rst, rsto] = [rst.map((_, i) => rsto[i] - q * rst[i]), rst];
  }
  return rsto;
}

function partB() {
  let numCards = 119315717514047n;
  let iters = 101741582076661n;
  let lcfs = lines.map((line) => {
    if(line.startsWith("deal into new stack")) {
      return new lcf(-1n,-1n);
    }
    else if(line.startsWith("cut ")) {
      let amount = BigInt(line.substr(4));
      return new lcf(1n,-amount);
    }
    let amount = BigInt(line.substr(20));
    return new lcf(amount,0n);
  });
  let masterLcf = lcfs.reduce((f, g) => g.compose(f,numCards), new lcf(1n,0n));
  masterLcf = pow_compose(masterLcf,BigInt(iters),numCards);
  let ans = masterLcf.solve(2020n,numCards)[0];
  while(ans < 0) {
    ans += numCards;
  }
  console.log(ans.toString());
}


rl.on('close', () => {
  try {
    console.log("Part A");
    newDeal();
    doLines();
    console.log(deck.findIndex(x => x === 2019));

    console.log("Part B");
    partB();
  } catch(e) {
    console.log(e);
  }
});