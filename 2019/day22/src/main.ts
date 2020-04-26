import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

  const numCards = 10007;

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

  function runTests() {
    test1();
    test2();
    test3();
  }

  //runTests();

  const lines = [];

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

rl.on('close', () => {
  try {
    console.log("Part A");
    doLines();
    console.log(deck.findIndex(x => x === 2019));

    console.log("Part B");
    for(let i = 0;i < 2000;++i) {
      doLines();
      console.log(deck[2020]);
    }
   
  } catch(e) {
    console.log(e);
  }
});