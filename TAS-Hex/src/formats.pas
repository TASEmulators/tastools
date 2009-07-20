unit formats;

interface

uses sysutils;

type
  ad = record
    where : integer;
    what : string;
  end;
  movie = record
    typ : integer;
    chars : string;
    head, post : array of byte;
    data : array[0..4] of array of longword;
    used : array[0..4] of byte;
    ads : array of ad;
    headl, datal, postl, adsl : integer;
  end;

  GMVheader = record
    gmv      : array[0..14] of char;
    ver      : byte;
    rerecs   : longword;
    pc       : array[0..1] of char;
    flags    : word;
    desc     : array[0..39] of char;
  end;

  M64header = record
    m64      : array[0..3] of char;
    ver      : longword;
    uid      : longint;
    frames   : longword;
    rerecs   : longword;
    fps      : byte;
    contrnum : byte;
    res1     : word;
    inputs   : longword;
    savetype : word;
    res2     : word;
    controlf : longword;
    res3     : array[0..159] of char;
    romname  : array[0..31] of char;
    crc32    : longword;
    country  : word;
    res4     : array[0..55] of char;
    videoplg : array[0..63] of char;
    soundplg : array[0..63] of char;
    inputplg : array[0..63] of char;
    rspplg   : array[0..63] of char;
    author   : array[0..221] of char;
    desc     : array[0..255] of char;
  end;

  SMVheader = record
    SMV      : array[0..3] of char;
    ver      : longword;
    uid      : longint;
    rerecs   : longword;
    frames   : longword;
    controlf : byte;
    movief   : byte;
    syncf    : byte;
    resf     : byte;
    saves    : longword;
    start    : longword;
  end;

  VBMheader = record
    VBM      : array[0..3] of char;
    ver      : longword;
    uid      : longint;
    frames   : longword;
    rerecs   : longword;
    savef    : byte;
    controlf : byte;
    systemf  : byte;
    emuf     : byte;
    savetype : longword;
    flashsize: longword;
    emutype  : longword;
    gamename : array[0..11] of char;
    null     : byte;
    romcrc   : byte;
    CRC16    : word;
    gamecode : longword;
    savestate: longword;
    start    : longword;
    author   : array[0..63] of char;
    desc     : array[0..127] of char;
  end;

  FCMheader = record
    FCM      : array[0..3] of char;
    version  : longword;
    flags    : byte;
    res1     : array[0..2] of byte;
    frames   : longword;
    rerecs   : longword;
    comlen   : longword;
    savex    : longword;
    start    : longword;
    crc      : array[0..15] of byte;
    emuver   : longword;
  end;

  FMVheader = record
    FCM      : array[0..3] of char;
    savef    : byte;
    controlf : byte;
    res1     : array[0..9] of byte;
//    rerecs   : longword;
//    res2     : word;
    emulator : array[0..63] of char;
    title    : array[0..63] of char;
  end;

function test (fn : string; var res, ver : integer) : integer;

procedure addad(var mov : movie; n : integer; s : string);

procedure  LoadFCM(var mov : movie; fn: string);
procedure  LoadFMV(var mov : movie; fn: string);
procedure  LoadGMV(var mov : movie; fn: string);
procedure  LoadM64(var mov : movie; fn: string);
procedure  LoadSMV(var mov : movie; fn: string);
procedure  LoadVBM(var mov : movie; fn: string);
procedure  LoadTXT(var mov : movie; fn: string);
function LoadMovie(var mov : movie; fn: string) : boolean;

procedure  SaveFCM(mov : movie; fn: string);
procedure  SaveFMV(mov : movie; fn: string);
procedure  SaveGMV(mov : movie; fn: string);
procedure  SaveM64(mov : movie; fn: string);
procedure  SaveSMV(mov : movie; fn: string);
procedure  SaveVBM(mov : movie; fn: string);
procedure  SaveTXT(mov : movie; fn: string);
function SaveMovie(mov : movie; fn: string) : boolean;

procedure clearmovie(var mov : movie);

implementation

procedure addad;
var a : integer;
begin
  for a := 0 to mov.adsl-1 do
  begin
    if mov.ads[a].where = n then
    if mov.ads[a].what = s then exit;
  end;
  inc(mov.adsl);
  setlength(mov.ads,mov.adsl);
  mov.ads[mov.adsl-1].where := n;
  mov.ads[mov.adsl-1].what := s;
end;

function test;
var f : file of char;
 ch : array[0..3] of char;
begin
  result := 0;
  res := 1;
  if not fileexists(fn) then exit;
  res := 2;
  assign(f,fn); reset(f);
  if filesize(f) < 4 then
  begin
    close(f);
    exit;
  end;
  blockread(f,ch[0],4);
  if ch = 'VBM'#$1A then result := 1;
  if ch = 'SMV'#$1A then result := 2;
  if ch = 'FCM'#$1A then result := 3;
  if ch = 'FMV'#$1A then result := 4;
  if ch = 'M64'#$1A then result := 5;
  if ch = 'Gens' then result := 6;
  if ch = 'TEXT' then result := 16;
  if result in [1..3,5] then
    blockread(f,ver,4);
  if result = 6 then
  begin
    blockread(f,ch[0],4);
    if ch <> ' Mov' then result := 0;
    blockread(f,ch[0],4);
    if ch <> 'ie T' then result := 0;
    blockread(f,ch[0],4);
    ver := ord(ch[3]);
    ch[3] := 'A';
    if ch <> 'ESTA' then result := 0;
  end;
  res := 0;
  close(f);
end;

procedure LoadFCM;
var
  f : file of byte;
  head : FCMHeader;
  data : array of byte;
  a,x,c,d,e,l : integer;
  b : byte;
  st : array[0..3] of byte;
begin
  mov.typ := 3;
  mov.chars := 'ABsS^v<>';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := head.start;
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],head.start);
  fillchar(mov.used,sizeof(mov.used),0);
  fillchar(st,sizeof(st),0);
  mov.datal := head.frames;
  for a := 0 to 3 do
  setlength(mov.data[a],head.frames);
  setlength(data,head.comlen);
  blockread(f,data[0],head.comlen);
  x := 0;
  mov.adsl := 0;
  setlength(mov.ads,0);
  l := 0;
  repeat
    b := data[x];
    inc(x);
    d := 0;
    c := (b shr 5) and 3;
    for e := 1 to c do
    begin
      d := d or data[x] shl ((e-1)*8);
      inc(x);
    end;
    for e := 1 to d do
    begin
      for c := 0 to 3 do mov.data[c][l] := st[c];
      inc(l);
      if l >= head.frames then break;
    end;
    if b shr 7 = 1 then
    begin
      if b and 31 = 1 then addad(mov,l,'$RESET');
      if b and 31 = 2 then addad(mov,l,'$POWER');
      if b and 31 = 7 then addad(mov,l,'$COIN');
      if b and 31 = 8 then addad(mov,l,'$DIPSWITCH');
      if b and 31 = 24 then addad(mov,l,'$FDS_INSERT');
      if b and 31 = 25 then addad(mov,l,'$FDS_EJECT');
      if b and 31 = 26 then addad(mov,l,'$FDS_SIDE');
    end
    else begin
      mov.used[(b shr 3) and 3] := 1;
      st[(b shr 3) and 3] := st[(b shr 3) and 3] xor (1 shl (b and 7));
    end;
  until (x >= head.comlen-1) or (l >= head.frames);
  mov.postl := filesize(f)-(head.start+head.comlen);
  setlength(mov.post,mov.postl);
  blockread(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure LoadFMV;
var
  f : file of byte;
  head : FMVHeader;
  data : array of byte;
  a,b,c : integer;
  cn, frames : integer;
  fds : boolean;
begin
  mov.typ := 4;
  mov.chars := '><^vBAsS';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := $90;
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],$90);
  fillchar(mov.used,sizeof(mov.used),0);
  mov.used[0] := 1 and (head.controlf shr 7);
  mov.used[1] := 1 and (head.controlf shr 6);
  fds := 1 and (head.controlf shr 5) > 0;
  cn := mov.used[0]+mov.used[1] + (1 and (head.controlf shr 5));
  frames := (filesize(f) - 144) div cn;
  mov.datal := frames;
  setlength(mov.data[0],frames);
  setlength(data,frames*cn);
  blockread(f,data[0],frames*cn);
  closefile(f);
  mov.adsl := 0;
  setlength(mov.ads,0);
  for a := 0 to frames-1 do
  begin
    b := 0;
    for c := 0 to 1 do
      if mov.used[c] > 0then
      begin
        mov.data[c][a] := data[a*cn+b];
        inc(b);
      end;
    if fds then
    if data[a*cn+b] <> 0 then
    begin
      addad(mov,a,'$FDS:'+inttostr(data[a*cn+b]));
    end;
  end;
  mov.postl := 0;
  setlength(mov.post,0);
end;

procedure LoadGMV;
var
  f : file of byte;
  head : GMVHeader;
  data : array of array[0..2] of byte;
  a : integer;
  frames : integer;
begin
  mov.typ := 6;
  mov.chars := '^v<>ABCSXYZM';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := $40;
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],$40);
  frames := (filesize(f)-$40) div 3;
  fillchar(mov.used,sizeof(mov.used),0);
  mov.used[0] := (ord(head.pc[0]) - $30) div 3;
  mov.used[1] := (ord(head.pc[1]) - $30) div 3;
  mov.used[2] := 1 and (head.flags shr 5);
  mov.datal := frames;
  setlength(mov.data[0],frames);
  setlength(mov.data[1],frames);
  if mov.used[2] > 0 then
  setlength(mov.data[2],frames);
  setlength(data,frames);
  blockread(f,data[0],frames*3);
  closefile(f);
  for a := 0 to frames-1 do
  begin
    if mov.used[2] = 0 then
    begin
      mov.data[0][a] := not (data[a][0] or (data[a][2] and $0F shl 8));
      mov.data[1][a] := not (data[a][1] or (data[a][2] and $F0 shl 4));
    end else begin
      mov.data[0][a] := not (data[a][0]);
      mov.data[1][a] := not (data[a][1]);
      mov.data[2][a] := not (data[a][2]);
    end;
  end;
  mov.postl := 0;
  setlength(mov.post,0);
  mov.adsl := 0;
  setlength(mov.ads,0);
end;

procedure LoadM64;
var
  f : file of byte;
  head : M64Header;
  data : array of longword;
  a : integer;
begin
  mov.typ := 5;
  mov.chars := '><v^SZBArlduRL12xxxxxxxxyyyyyyyy';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := $400 - $200*ord(head.ver in [1..2]);
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],$400 - $200*ord(head.ver in [1..2]));
  fillchar(mov.used,sizeof(mov.used),0);
  mov.used[0] := 2;
  mov.datal := head.inputs;
  setlength(mov.data[0],head.inputs);
  setlength(data,head.inputs);
  blockread(f,data[0],head.inputs * 4);
  for a := 0 to head.inputs-1 do
    mov.data[0][a] := data[a];
  mov.postl := filesize(f)-($400 - $200*ord(head.ver in [1..2])+head.inputs*4);
  setlength(mov.post,mov.postl);
  blockread(f,mov.post[0],mov.postl);
  closefile(f);
  mov.adsl := 0;
  setlength(mov.ads,0);
end;

procedure LoadVBM;
var
  f : file of byte;
  head : VBMHeader;
  data : array of word;
  a : integer;
begin
  mov.typ := 1;
  mov.chars := 'ABsS><^vRL123456';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := head.start;
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],head.start);
  fillchar(mov.used,sizeof(mov.used),0);
  mov.used[0] := 2;
  mov.datal := head.frames+1;
  setlength(mov.data[0],head.frames+1);
  setlength(data,head.frames+1);
  blockread(f,data[0],head.frames * 2+2);
  for a := 0 to head.frames do
    mov.data[0][a] := data[a];
  mov.postl := filesize(f)-(head.start+head.frames*2+2);
  setlength(mov.post,mov.postl);
  blockread(f,mov.post[0],mov.postl);
  closefile(f);
  mov.adsl := 0;
  setlength(mov.ads,0);
end;

procedure LoadSMV;
var
  f : file of byte;
  head : SMVHeader;
  data : array of word;
  cn : byte;
  a,b,c : integer;
begin
  mov.typ := 2;
  mov.chars := '1234RLXA><v^SsYB';
  assignfile(f, fn); reset(f);
  blockread(f,head,sizeof(head));
  mov.headl := head.start;
  setlength(mov.head,mov.headl);
  seek(f,0);
  blockread(f,mov.head[0],head.start);
  for a := 0 to 4 do
    mov.used[a] := ord(head.controlf and (1 shl a) > 0)*2;
  mov.datal := head.frames+1;
  cn := 0;
  for a := 0 to 4 do
    if mov.used[a] > 0 then
    begin
      setlength(mov.data[a],head.frames+1);
      inc(cn);
    end;
  setlength(data,(head.frames+1)*cn);
  blockread(f,data[0],(head.frames+1)*cn*2);
  for a := 0 to head.frames do
  begin
    b := 0;
    for c := 0 to 4 do
      if mov.used[c] > 0 then
      begin
        mov.data[c][a] := data[a*cn+b];
        inc(b);
      end;
  end;
  mov.postl := filesize(f)-(head.start+(head.frames+1)*2*cn);
  setlength(mov.post,mov.postl);
  blockread(f,mov.post[0],mov.postl);
  closefile(f);
  mov.adsl := 0;
  setlength(mov.ads,0);
end;

procedure readdata(var mov : movie; n : integer; fn : string);
var
  f : text;
  s,ss : string;
  l,x : integer;
  a,c,d,e : integer;
  b : byte;
begin
  assign(f,fn); reset(f);
  readln(f,s);
  mov.used[n] := 0;
  if copy(s,1,5) = 'CHARS' then mov.chars := copy(s,7,32) else exit;
  mov.used[n] := length(mov.chars) div 8;
  l := 0;
  repeat
    readln(f,s);
    s := s + ' ';
    if s[1] = '$' then continue;
    while pos('|',s) > 0 do delete(s,1,pos('|',s));
    if pos(':',s) > 0 then
    begin
      s := trim(copy(s,pos(':',s)+1,6));
      inc(l,strtoint(s));
    end else inc(l);
  until eof(f);
  if l > mov.datal then
  begin
    mov.datal := l;
    for a := 0 to 4 do if mov.used[a] > 0 then setlength(mov.data[a],l);
  end else l := mov.datal;
  setlength(mov.data[n],l);
  close(f); reset(f); readln(f,s);
  x := 0;
  repeat
    c := 0;
    readln(f,s); s := s + '   ';
    while pos('|',s) > 0 do delete(s,1,pos('|',s));
    if (s[1] = '$') or (copy(s,1,2) = '//') then addad(mov,x,trim(s)) else
    begin
      while (s[1] <> ':') and (length(s) > 0) do
      begin
        while copy(s,1,1) = ' ' do delete(s,1,1);
        if length(s) = 0 then break;
        if s[1] = ':' then break;
        if pos(s[1],mov.chars) = 0 then
        begin
          delete(s,1,1);
          continue;
        end;
        ss := '1';
        if s[2] = '=' then
        begin
          delete(s,2,1);
          ss := '';
          b := 2;
          while s[b] in ['0'..'9','-'] do
          begin
            ss := ss + s[b];
            delete(s,b,1);
          end;
        end;
        if ss = '' then b := 1 else b := strtoint(ss);
        e := pos(s[1],mov.chars)-1;
//        for a := 1 to length(mov.chars) do if mov.chars[a] = s[1] then e := a-1;
        c := c or (b shl e);
        delete(s,1,1);
      end;
      d := 1;
      if length(s) > 0 then
      begin
        delete(s,1,1);
        d := strtoint(trim(s));
      end;
      for a := 1 to d do
      begin
        mov.data[n][x] := c;
        inc(x);
      end;
    end;
  until eof(f);
end;

procedure LoadTXT;
var
  f : text;
  s : string;
  a : integer;
  b : byte;
begin
  assign(f,fn); reset(f);
  readln(f,s);
  if s <> 'TEXT' then exit;
  repeat
    readln(f,s);
    if copy(s,1,4) = 'TYPE' then mov.typ := strtoint(copy(s,6,3));
  until s = 'HEADER';
  readln(f,s);
  mov.headl := length(s) div 2;
  setlength(mov.head,mov.headl);
  s := uppercase(s);
  for a := 0 to mov.headl-1 do
  begin
    b := ord(s[a*2+1]) - ord(s[a*2+1] > '9')*7 - $30;
    b := (b shl 4) or (ord(s[a*2+2]) - ord(s[a*2+2] > '9')*7 - $30);
    mov.head[a] := b;
  end;
  repeat
    readln(f,s);
    if copy(s,1,4) = 'TYPE' then mov.typ := strtoint(copy(s,6,3));
  until s = 'POST';
  readln(f,s);
  mov.postl := length(s) div 2;
  setlength(mov.post,mov.postl);
  s := uppercase(s);
  for a := 0 to mov.postl-1 do
  begin
    b := ord(s[a*2+1]) - ord(s[a*2+1] > '9')*7 - $30;
    b := (b shl 4) or (ord(s[a*2+2]) - ord(s[a*2+2] > '9')*7 - $30);
    mov.post[a] := b;
  end;
  repeat
    readln(f,s);
    if copy(s,1,4) = 'TYPE' then mov.typ := strtoint(copy(s,6,3));
  until s = 'CONTROLLERS';
  b := 0;
  while not eof(f) do
  begin
    readln(f,s);
    readdata(mov,b,s);
    inc(b);
  end;
end;

function LoadMovie;
var
  typ,res,ver : integer;
begin
  result := false;
  typ := test(fn,res,ver);
  if typ = 0 then exit;
  if typ = 1 then
  LoadVBM(mov,fn);
  if typ = 2 then LoadSMV(mov,fn);
  if typ = 3 then LoadFCM(mov,fn);
  if typ = 4 then LoadFMV(mov,fn);
  if typ = 5 then LoadM64(mov,fn);
  if typ = 6 then LoadGMV(mov,fn);
  if typ = 16 then LoadTXT(mov,fn);
  result := true;
end;

function SaveMovie;
var
  typ : integer;
begin
  result := false;
  typ := mov.typ;
  if typ = 0 then exit;
  if typ = 1 then
  SaveVBM(mov,fn);
  if typ = 2 then SaveSMV(mov,fn);
  if typ = 3 then SaveFCM(mov,fn);
  if typ = 4 then SaveFMV(mov,fn);
  if typ = 5 then SaveM64(mov,fn);
  if typ = 6 then SaveGMV(mov,fn);
  if typ = 16 then SaveTXT(mov,fn);
  result := true;
end;

procedure writedata(mov : movie; n : integer; fn : string);
var
  a,b,d,x : integer;
  c : longword;
  y : shortint;
  f : text;
begin
  assign(f,fn); rewrite(f);
  writeln(f,'CHARS=' + mov.chars);
  for a := 0 to mov.datal-1 do
  begin
    for x := 0 to mov.adsl-1 do if mov.ads[x].where = a then writeln(f,mov.ads[x].what);
    write(f,a:10,'|');
    c := mov.data[n][a];
    x := 0;
    d := 0;
    for b := 1 to length(mov.chars) do
    begin
      if b = 1 then inc(x) else
      if mov.chars[b] <> mov.chars[b-1] then
      begin
        if d <> 0 then
        if x = 1 then
          write(f,mov.chars[b-1])
        else
        begin
          y := 0;
          for x := 0 to 7 do
          begin
            y := y shl 1 or (d and 1);
            d := d shr 1;
          end;
          write(f,' ',mov.chars[b-1],'=',inttostr(y),' ');
        end;
        x := 1;
        d := 0;
      end else
      begin
        d := d shl 1;
        inc(x);
      end;
      d := d or (c and 1);
      c := c shr 1;
    end;
    b := length(mov.chars);
    if d <> 0 then
      if x = 1 then
        write(f,mov.chars[b])
      else
        begin
          y := 0;
          for x := 0 to 7 do
          begin
            y := y shl 1 or (d and 1);
            d := d shr 1;
          end;
          write(f,' ',mov.chars[b],'=',inttostr(y),' ');
        end;
    writeln(f);
  end;
  close(f);
end;

procedure SaveTXT;
var
  f : text;
  s : string;
  a : integer;
begin
  assign(f,fn); rewrite(f);
  writeln(f,'TEXT');
  writeln(f,'TYPE=' + inttostr(mov.typ));
  writeln(f,'HEADER');
  for a := 0 to mov.headl-1 do
  begin
    write(f,inttohex(mov.head[a],2));
  end;
  writeln(f);
  writeln(f,'POST');
  for a := 0 to mov.postl-1 do
  begin
    write(f,inttohex(mov.post[a],2));
  end;
  writeln(f);
  writeln(f,'CONTROLLERS');
  for a := 0 to 4 do
    if mov.used[a] > 0 then
    begin
      s := changefileext(fn,'.c'+inttostr(a+1)+'.txt');
      writeln(f,s);
      writedata(mov,a,s);
    end;
  close(f);
end;

procedure SaveVBM;
var
  head : ^VBMHeader;
  f : file of byte;
  data : array of word;
  a : integer;
begin
  head := @mov.head[0];
  head.frames := mov.datal-1;
  setlength(data,mov.datal);
  for a := 0 to mov.datal-1 do data[a] := mov.data[0][a];
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],mov.datal*2);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure SaveSMV;
var
  head : ^SMVHeader;
  f : file of byte;
  data : array of word;
  a,b,c,cn : integer;
begin
  head := @mov.head[0];
  head.frames := mov.datal-1;
  cn := 0;
  b := 0;
  for a := 0 to 4 do
    if mov.used[a] > 0 then
    begin
      inc(cn);
      b := b or (1 shl a)
    end;
  head.controlf := b;
  setlength(data,mov.datal*cn);
  for a := 0 to mov.datal-1 do
  begin
    b := 0;
    for c := 0 to 4 do if mov.used[c] > 0 then
    begin
      data[a*cn+b] := mov.data[c][a];
      inc(b);
    end;
  end;
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],mov.datal*2*cn);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure SaveFMV;
var
  head : ^FMVHeader;
  f : file of byte;
  data : array of byte;
  a,b,c,cn : integer;
begin
  head := @mov.head[0];
//  head.frames := mov.datal-1;
  cn := 0;
  b := 0;
  for a := 0 to 1 do
    if mov.used[a] > 0 then
    begin
      inc(cn);
      b := b or (1 shl (7-a))
    end;
  c := 0;
  for a := 0 to mov.adsl-1 do if copy(mov.ads[a].what,1,5) = '$FDS:' then c := 1;
  if c = 1 then
  begin
    b := b or (1 shl 5);
    inc(cn);
  end;
  head.controlf := b;
  setlength(data,mov.datal*cn);
  for a := 0 to mov.datal-1 do
  begin
    b := 0;
    for c := 0 to 1 do if mov.used[c] > 0 then
    begin
      data[a*cn+b] := mov.data[c][a];
      inc(b);
    end;
    if cn - 1 > b then
    begin
      for c := 0 to mov.adsl-1 do
        if copy(mov.ads[c].what,5,1) = '$FDS:' then
          data[a*cn+b] := strtoint(copy(mov.ads[c].what,6,3));
    end;
  end;
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],mov.datal*cn);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure SaveGMV;
var
  head : ^GMVHeader;
  f : file of byte;
  data : array of array[0..2] of byte;
  a : integer;
begin
  head := @mov.head[0];
//  head.frames := mov.datal-1;
  head.pc[0] := chr($30+mov.used[0]*3);
  head.pc[1] := chr($30+mov.used[1]*3);
  head.flags := (head.flags and $DF) or (ord(mov.used[2] > 0) shl 5);
  setlength(data,mov.datal);
  for a := 0 to mov.datal-1 do
  begin
    data[a][0] := not mov.data[0][a];
    data[a][1] := not mov.data[1][a];
    if mov.used[2] > 0 then
      data[a][2] := not mov.data[2][a]
    else
      data[a][2] := not (((mov.data[0][a] shr 8) and 15) or (((mov.data[1][a] shr 8) and 15)shl 4));
  end;
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],mov.datal*3);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure SaveM64;
var
  head : ^M64Header;
  f : file of byte;
  data : array of longword;
  a : integer;
begin
  head := @mov.head[0];
  head.inputs := mov.datal;
  setlength(data,mov.datal);
  for a := 0 to mov.datal-1 do data[a] := mov.data[0][a];
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],mov.datal*4);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure SaveFCM;
function intl (n : integer):integer;
begin
result := 0;
if n = 0 then exit;
result := 1;
if n < 256 then exit;
result := 2;
if n < 65536 then exit;
result := 3;
end;
var
  head : ^FCMHeader;
  f : file of byte;
  data : array of byte;
  datal : integer;
  a,b,c,d,e,x : integer;
  od : array[0..3] of byte;
begin
  head := @mov.head[0];
  head.frames := mov.datal;
  c := 0;
  datal := 0;
  for a := 0 to 3 do od[a] := 0;
  for a := 0 to mov.datal-1 do
  begin
    for b := 0 to mov.adsl-1 do if mov.ads[b].where = a then
    begin
      inc(datal,1+intl(c));
      c := 0;
    end;
    for b := 0 to 3 do
    begin
      if mov.used[b] > 0 then
      if mov.data[b][a] <> od[b] then
      begin
        d := 0;
        for e := 0 to 7 do inc(d,((mov.data[b][a] xor od[b]) shr e) and 1);
        inc(datal,d+intl(c));
        c := 0;
      end;
    end;
//    data[a] := mov.data[0][a];
    inc(c);
    for d := 0 to 3 do if mov.used[d]>0 then od[d] := mov.data[d][a];
  end;
  inc(datal,1+intl(c));
  setlength(data,datal);
  head.comlen := datal;
  c := 0;
  datal := 0;
  for a := 0 to 3 do od[a] := 0;
  for a := 0 to mov.datal-1 do
  begin
    for b := 0 to mov.adsl-1 do if mov.ads[b].where = a then
    begin
      e := 0;
      if mov.ads[b].what = '$RESET' then e := 1;
      if mov.ads[b].what = '$POWER' then e := 2;
      if mov.ads[b].what = '$COIN' then e := 7;
      if mov.ads[b].what = '$DIPSWITCH' then e := 8;
      if mov.ads[b].what = '$FDS_INSERT' then e := 24;
      if mov.ads[b].what = '$FDS_EJECT' then e := 25;
      if mov.ads[b].what = '$FDS_SIDE' then e := 26;
      data[datal] := 1 shl 7 + intl(c) shl 5 + e;
      inc(datal);
      for e := 0 to intl(c)-1 do
      begin
        data[datal] := c and $ff;
        c := c shr 8;
        inc(datal);
      end;
      c := 0;
    end;
    for b := 0 to 3 do
    begin
      if mov.used[b] > 0 then
      if mov.data[b][a] <> od[b] then
      begin
//        d := 0;
        for e := 0 to 7 do if ((mov.data[b][a] xor od[b]) shr e) and 1 = 1 then
        begin
          data[datal] := intl(c) shl 5 + b shl 3 + e;
          inc(datal);
          for x := 0 to intl(c)-1 do
          begin
            data[datal] := c and $ff;
            c := c shr 8;
            inc(datal);
          end;
          c := 0;
        end;
      end;
    end;
//    data[a] := mov.data[0][a];
    inc(c);
    for d := 0 to 3 do if mov.used[d]>0 then od[d] := mov.data[d][a];
  end;
  data[datal] := 1 shl 7 + intl(c) shl 5;
  inc(datal);
  for e := 0 to intl(c)-1 do
  begin
    data[datal] := c and $ff;
    c := c shr 8;
    inc(datal);
  end;
//  c := 0;
  assign(f,fn); rewrite(f);
  blockwrite(f,mov.head[0],mov.headl);
  blockwrite(f,data[0],datal);
  blockwrite(f,mov.post[0],mov.postl);
  closefile(f);
end;

procedure clearmovie;
begin
  mov.typ := 0;
  mov.chars := '';
  setlength(mov.head,0);
  setlength(mov.post,0);
  setlength(mov.ads,0);
  setlength(mov.data[0],0);
  setlength(mov.data[1],0);
  setlength(mov.data[2],0);
  setlength(mov.data[3],0);
  setlength(mov.data[4],0);
  mov.used [0] := 0;
  mov.used [1] := 0;
  mov.used [2] := 0;
  mov.used [3] := 0;
  mov.used [4] := 0;
  mov.headl := 0;
  mov.datal := 0;
  mov.postl := 0;
  mov.adsl := 0;
end;
end.
