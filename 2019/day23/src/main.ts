import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

  enum Instr {
    Add = 1,
    Multiply = 2,
    Input = 3,
    Output = 4,
    JumpIfTrue = 5,
    JumpIfFalse = 6,
    LessThan = 7,
    Equals = 8,
    AdjustRelativeBase = 9,
    Halt = 99
  };
  
  enum Mode { Position = 0, Immediate = 1, Relative = 2 };
  class DecodedInstr {
    public instr: Instr;
    public instrMode: Mode;
    public param1Mode: Mode;
    public param2Mode: Mode;
    public param3Mode: Mode;
  }
  
  function decodeInstr(n: bigint): DecodedInstr {
    if(typeof n !== 'bigint') {
      console.log("here");
    }
    let di = new DecodedInstr();
    let de = n % 100n;
    let c = (n / 100n) % 10n;
    let b = (n / 1000n) % 10n;
    let a = (n / 10000n) % 10n;
  
    di.param1Mode = Mode[Object.keys(Mode).find(key => Mode[key] == c)];
    di.param2Mode = Mode[Object.keys(Mode).find(key => Mode[key] == b)];
    di.param3Mode = Mode[Object.keys(Mode).find(key => Mode[key] == a)];
  
    let instrKey : string = null;
    if(de != BigInt(Instr.Halt)) {
      let e = de % 10n;
      instrKey = Object.keys(Instr).find(key => Instr[key] == e); 
    }
    else {
      instrKey = Object.keys(Instr).find(key => Instr[key] == de); 
    }
    
    if(instrKey == null) {
      throw new Error(`Invalid instruction ${de}`);
    }
    di.instr = Instr[instrKey];
    if (di.instr !== Instr.Halt) {
      let d = de / 10n;
      di.instrMode = (d == 0n) ? Mode.Position : Mode.Immediate;
    } else {
      di.instrMode = Mode.Position;
    }
    return di;
  }

  class Simulator
  {
    private arr : bigint[];
    private ip : number;
    private output : bigint[];
    private input : bigint[];
    private outputCallback : (v: bigint, s: Simulator) => void;
    private inputCallback : () => bigint;
    private maxOutputLength : number;
    private relativeBase : number;
    public outputBuffer : bigint[];
  
    constructor(arr : bigint[]) {
      this.arr = arr.slice();
      this.ip = 0;
      this.input = [];
      this.output = [];
      this.maxOutputLength = null;
      this.relativeBase = 0;
      this.outputBuffer = [];
    }
  
    readMem(address : number) : bigint {
      if(this.arr[address] == null) {
        this.arr[address] = 0n;
      }
      return this.arr[address];
    }
  
    readMem2(address : number) : number {
      if(this.arr[address] == null) {
        this.arr[address] = 0n;
      }
      return Number(this.arr[address].toString());
    }
  
    writeMem(address : number,value : bigint,mode: Mode) : void {
      if(mode === Mode.Position) {
        this.arr[address] = value;
      } else if(mode === Mode.Relative) {
        this.arr[this.relativeBase + address] = value;
      }
      else {
        throw new Error(`Unhandled mode ${mode}`);
      }
    }
  
    setMaxOutputLength(n : number) {
      this.maxOutputLength = n;
    }
  
    getLastOutput() : bigint {
      if (this.output.length === 0) {
        throw new Error("no output");
      }
      return this.output[this.output.length-1];
    }
  
    setOutputCallback(callback : (v: bigint, s: Simulator) => void) {
      this.outputCallback = callback;
    }
  
    setInputCallback(callback : () => bigint) {
      this.inputCallback = callback;
    }
  
    getInput() : bigint {
      if(this.input.length === 0 && this.inputCallback != null) {
        let newValue = this.inputCallback();
        if(newValue != null) {
          this.input.push(newValue);
        }
      }
      if(this.input.length === 0) {
        throw new Error("No Input");
      }
      return this.input.shift();
    }
  
    pushInput(v : bigint) {
      this.input.push(v);
    }
  
    setOutput(v : bigint) {
      this.output.push(v);
      if(this.outputCallback != null) {
        this.outputCallback(v, this);
      }
    }
  
    private runStep() {
      return this.runStepInternal();
    }
  
    private runStepInternal() {
      let arr = this.arr;
      let ip = this.ip;
      let di = decodeInstr(this.readMem(ip));
      let opCode = di.instr;
    
      if (opCode === Instr.Add) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        this.writeMem(this.readMem2(ip + 3),v1+v2,di.param3Mode);
        return ip + 4;
      } else if (opCode === Instr.Multiply) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        this.writeMem(this.readMem2(ip + 3),v1*v2,di.param3Mode);
        return ip + 4;
      } else if (opCode === Instr.Input) {
        this.writeMem(this.readMem2(ip+1),this.getInput(),di.param1Mode);
        return ip + 2;
      } else if (opCode === Instr.Output) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        this.setOutput(v1);
        return ip + 2;
      } else if (opCode === Instr.JumpIfTrue) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        return v1 != 0n ? Number(v2.toString()) : ip + 3;
      } else if (opCode === Instr.JumpIfFalse) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        return v1 == 0n ? Number(v2.toString()) : ip + 3;
      } else if (opCode === Instr.LessThan) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        this.writeMem(this.readMem2(ip + 3),v1 < v2 ? 1n : 0n,di.param3Mode);
        return ip + 4;
      } else if (opCode === Instr.Equals) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let v2 = this.getValue(ip + 2, di.param2Mode);
        this.writeMem(this.readMem2(ip + 3),v1 === v2 ? 1n : 0n,di.param3Mode);
        return ip + 4;
      }
      else if(opCode === Instr.AdjustRelativeBase) {
        let v1 = this.getValue(ip + 1, di.param1Mode);
        let offset = Number(v1.toString());
        if(offset.toString() !== v1.toString()) {
          console.assert(false);
        }
        this.relativeBase += offset;
        return ip + 2;
      }
      else if (opCode === Instr.Halt) {
        return -1;
      }
      throw new Error(`Unknown opCode ${opCode} at ip ${ip}, ${arr}`);
    }
  
    isFinished() : boolean {
      if(this.ip === -1) return true;
      if(this.maxOutputLength != null) {
        return this.output.length >= this.maxOutputLength;
      }
      return false;
    }
  
    isPaused() : boolean {
      if(this.ip === -1) return false;
      if(this.inputCallback != null) return false;
      let di = decodeInstr(this.readMem(this.ip));
      let opCode = di.instr;
      if(opCode === Instr.Input) {
        return this.input.length === 0;
      }
      return false;
    }
  
    runSimInternal() {
      while(!this.isFinished() && !this.isPaused()) {
        this.ip = this.runStep();
      }
    }

    step() {
      if(!this.isFinished() && !this.isPaused()) {
        this.ip = this.runStep();
      }
    }
  
    getValue(offset: number, mode: Mode): bigint {
      if (mode === Mode.Position) {
        let index = this.readMem2(offset);
        return this.readMem(index);
      } else if (mode === Mode.Immediate) {
        return this.readMem(offset);
      } else if(mode === Mode.Relative) {
        let newOffset = this.relativeBase + this.readMem2(offset);
        return this.readMem(newOffset);
      }
      throw new Error(`unknown mode ${mode}`);
    }

    reset() : void {
      this.ip = 0;
    }
  }

  let arr = [];
  rl.on('line', (line: string) => {
    let instr = line.split(',').map(x => BigInt(x));
    instr.forEach(i => arr.push(i));
  });


  function partA() {
    let isDone = false;
    const sims : Simulator[] = [];
    for(let i = 0;i < 50;++i) {  
      const sim = new Simulator(arr); 
      sim.pushInput(BigInt(i));
      sim.setInputCallback(() => {
        //console.log(`${sims.findIndex((s) => s === sim)} no input`);
        return -1n;
      });
      sim.setOutputCallback((v, l) => {
        l.outputBuffer.push(v);
        if(l.outputBuffer.length === 3) {
          const addr = Number(sim.outputBuffer[0]);
          if(addr == 255) {
            console.log(sim.outputBuffer[2]);
            isDone = true;
          } 
          else {
            //console.log(`${sims.findIndex(s => s === l)} sending ${addr} ${l.outputBuffer[1]} ${l.outputBuffer[2]}`);
            sims[addr].pushInput(l.outputBuffer[1]);
            sims[addr].pushInput(l.outputBuffer[2]);
          }
          l.outputBuffer.pop();
          l.outputBuffer.pop();
          l.outputBuffer.pop();
        }
      });
      sims.push(sim);
    }
    while(!isDone) {
      for(let i = 0;!isDone && i < sims.length;++i) {
        sims[i].step();
      }
    }
  }

  function partB() {
    let naty = new Set<string>();
    let natPacket = [];
    let isIdle = new Array(50).fill(false);
    let isDone = false;
    const sims : Simulator[] = [];
    for(let i = 0;i < 50;++i) {  
      const sim = new Simulator(arr); 
      sim.pushInput(BigInt(i));
      sim.setInputCallback(() => {
        const index = sims.findIndex((s) => s === sim);
        isIdle[index] = true;
        return -1n;
      });
      sim.setOutputCallback((v, l) => {
        l.outputBuffer.push(v);
        if(l.outputBuffer.length === 3) {
          const addr = Number(sim.outputBuffer[0]);
          if(addr == 255) {
            natPacket = sim.outputBuffer.slice(1)
          } 
          else {
            isIdle[addr] = false;
            sims[addr].pushInput(l.outputBuffer[1]);
            sims[addr].pushInput(l.outputBuffer[2]);
          }
          l.outputBuffer.pop();
          l.outputBuffer.pop();
          l.outputBuffer.pop();
        }
      });
      sims.push(sim);
    }
    let numIdle = 0;
    while(!isDone) {
      for(let i = 0;!isDone && i < sims.length;++i) {
        sims[i].step();
      }
      if(isIdle.every(x => x)) {
        numIdle += 1;
        if(numIdle > 10000) {
          console.log(`NAT transmitting ${natPacket}`);
          const y = natPacket[1].toString();
          if(naty.has(y)) {
            console.log(y);
            isDone = true;
          }
          naty.add(y);
          isIdle[0] = false;
          sims[0].pushInput(natPacket[0]);
          sims[0].pushInput(natPacket[1]);
        }
      }
      else {
        numIdle = 0;
      }
    }
  }

  rl.on('close', () => {
    try {
      console.log("Part A");
      partA();
      console.log("Part B");
      partB();
    } catch(e) {
      console.log(e);
    }
  }); 