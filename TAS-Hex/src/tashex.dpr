program tashex;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  formats;

var
  res : integer;
  chars : string = '00000000111111112222222233333333';
  mov : array[0..9] of movie;
  s,s1 : string;
  f : text;
  a,b,c,d,e,g : integer;

const
  ext : array[0..6] of string =
  ('.txt','.vbm','.smv','.fcm','.fmv','.m64','.gmv');

procedure delfr(var mov : movie; x,n : integer);
var
  a,b : integer;
begin
  for a := x to mov.datal - n - 1 do
  begin
    for b := 0 to 4 do if mov.used[b] > 0 then mov.data[b][a] := mov.data[b][a+n];
  end;
  mov.datal := mov.datal - n;
  for b := 0 to 4 do if mov.used[b] > 0 then setlength(mov.data[b],mov.datal);
end;

procedure insfr(var mov : movie; x,n : integer);
var
  a,b : integer;
begin
  mov.datal := mov.datal + n;
  for b := 0 to 4 do if mov.used[b] > 0 then setlength(mov.data[b],mov.datal);
  for a := mov.datal - 1 downto x + n do
  begin
    for b := 0 to 4 do if mov.used[b] > 0 then mov.data[b][a] := mov.data[b][a-n];
  end;
  for a := x to x + n - 1 do
    for b := 0 to 4 do if mov.used[b] > 0 then mov.data[b][a] := 0;
end;

procedure setfr(var mov : movie; x,n,cont,bit,val : integer);
var a : integer;
begin
  for a := x to x+n-1 do
    mov.data[cont-1][a] := (mov.data[cont-1][a] and ($FFFFFFFF xor (1 shl bit))) or ((val and 1)shl bit);
end;

procedure copyfr(var mov : movie; x1 : integer; mov2 : movie; x2,l : integer);
var a,b : integer;
begin
  insfr(mov,x1,l);
  for a := 0 to l-1 do
    for b := 0 to 4 do
      if mov.used[b] > 0 then
        if mov2.used[b] > 0 then
          mov.data[b][a+x1] := mov2.data[b][a+x2];
end;

begin
  writeln('TAS command line editor v 0.0.1');
  if paramcount = 0 then
  begin
//    writeln('Usage:');
//    writeln('   '+extractfilename(paramstr(0))+' <switch> <options>');
//    writeln('   -t <filename> - decodes file to text');
//    writeln('   -c <filename> - encodes text files into movie');
    exit;
  end;
  for a := 0 to 9 do clearmovie(mov[a]);
  if paramcount = 1 then
  begin
    s := paramstr(1);
    if not fileexists(s) then exit;
    assign(f,s); reset(f);
    repeat
      readln(f,s);
      while pos('//',s) > 0 do delete(s,pos('//',s),length(s));
      s := trim(s) + ' ';
      s1 := lowercase(copy(s,1,pos(' ',s)-1));
      delete(s,1,length(s1)+1);
      if s1 = 'open' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s1 := trim(s);
        clearmovie(mov[a]);
        loadmovie(mov[a],s1);
      end else if s1 = 'save' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s1 := trim(s);
        if extractfileext(s1) = '' then
          s1 := s1 + ext[mov[a].typ];
        savemovie(mov[a],s1);
      end else if s1 = 'savetext' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s1 := trim(s);
        delete(s,1,length(s1)+1);
        if extractfileext(s1) = '' then
          s1 := s1 + '.txt';
        saveTXT(mov[a],s1);
      end else if s1 = 'delete' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        delfr(mov[a],b,c);
      end else if s1 = 'insert' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        insfr(mov[a],b,c);
      end else if s1 = 'set' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        d := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        e := strtoint(s1);
        setfr(mov[a],b,c,d,e,1);
      end else if s1 = 'unset' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        d := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        e := strtoint(s1);
        setfr(mov[a],b,c,d,e,0);
      end else if s1 = 'copy' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        d := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        e := strtoint(s1);
        copyfr(mov[a],b,mov[c],d,e);
      end else if s1 = 'replace' then begin
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        a := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        b := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        c := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        d := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        e := strtoint(s1);
        s := trim(s) + ' ';
        s1 := lowercase(copy(s,1,pos(' ',s)-1));
        delete(s,1,length(s1)+1);
        g := strtoint(s1);
        delfr(mov[a],b,c-b+1);
        copyfr(mov[a],b,mov[d],e,g-e+1);
//      end else if s1 = '' then begin
      end;
    until eof(f);
    close(f);
  end;
  if paramcount >= 2 then
  begin
    a := 1;
    s1 := paramstr(a); inc(a);
    if s1 = '-t' then
    begin
      s1 := ' '; if a <= paramcount then s1 := paramstr(a); inc(a);
      loadmovie(mov[0],s1);
      s1 := changefileext(s1,'.txt');
      if a <= paramcount then s1 := paramstr(a); inc(a);
      if extractfileext(s1) = '' then
        s1 := s1 + '.txt';
      saveTXT(mov[0],s1);
    end;
    if s1 = '-c' then
    begin
      s1 := ' '; if a <= paramcount then s1 := paramstr(a); inc(a);
      loadmovie(mov[0],s1);
      s1 := changefileext(s1,ext[mov[0].typ]);
      if a <= paramcount then s1 := paramstr(a);
      if extractfileext(s1) = '' then
        s1 := s1 + ext[mov[0].typ];
      savemovie(mov[0],s1);
    end;
    if s1 = '-r' then
    begin
      s := paramstr(2);
      loadmovie(mov[0],s);
      b := strtoint(paramstr(3));
      c := strtoint(paramstr(4));
      s1 := paramstr(5);
      loadmovie(mov[1],s1);
      e := strtoint(paramstr(6));
      g := strtoint(paramstr(7));
      delfr(mov[0],b,c-b+1);
      copyfr(mov[0],b,mov[1],e,g-e+1);
      s1 := paramstr(8);
      if s1 = '' then s1 := s;
      savemovie(mov[0],s1);
    end;
    if s1 = '-d' then
    begin
      s := paramstr(2);
      loadmovie(mov[0],s);
      b := strtoint(paramstr(3));
      c := strtoint(paramstr(4));
      s1 := paramstr(5);
      delfr(mov[0],b,c);
      if s1 = '' then s1 := s;
      savemovie(mov[0],s1);
    end;
  end;
  res := 0;
  if res <> 0 then
  begin
    if res = 1 then writeln ('File does not exist');
    if res = 2 then writeln ('File is too small');
  end;
//  readln;
end.
