/*
 * Blah blah blah.
 * 
 * Joe Lennox - JLennox[at]gmail[dt]com - http://www.red-stars.net
 */

using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.IO;
using System.Security.Cryptography;
using System.Data;
using SQLite.NET;
using ICSharpCode.SharpZipLib.Zip;
using ICSharpCode.SharpZipLib;
using ICSharpCode.SharpZipLib.Zip.Compression.Streams;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows;
using System.Security.Permissions;
using System.Windows.Forms;

namespace emuMovFront {
	class Args {	//This is not finished and is for future use.
		private Dictionary<string, string> _dict;
		private void _parse(string[] args, char define) {

			string line = string.Join(" ", args);
			string curCmd = null, curParam = null;
			bool inquotes = false;
			bool inparam = false;
			bool incmd = false;
			for (int i = 0; i <= line.Length; i++) {
				if ((line[i] == define) && (inparam == false)) {
					incmd = true; i++;
				} else if ((line[i] == ' ') && (incmd == true)) {
					incmd = false; inparam = true; i++;
					if (line[i+1] == '"') {
						i++; inquotes=true;
					}
				} else if ((line[i] == '"') && (inquotes==true)) {
					inparam = false; inquotes = false;
				}
				if (incmd)
					curCmd += line[i];
				else if (inparam)
					curParam += line[i];
			}
		}
		private bool _isquote(char chr) {
			if ((chr == '"') || (chr == '\''))
				return true;
			return false;
		}
		public Args(string[] args) { _parse(args, '/'); }
		public Args(string[] args, char define) { _parse(args, define); }
	}
	class Program {
		public static database db = null;
		private INI _settings;
		private static double _version = .4;
		private static double _dbversion = 1;
		private enum _promptdlgtype { rom, emu, dir }
		[STAThread]
		static void Main(string[] args) {
			new Program(args);
		}
		private string _getprogdir() {
			return Directory.GetParent(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName).ToString();
		}
		private enum _mainmenuoptions : int { adddir, about, exit }
		private string[] _mainmenustrings = new string[] {
			"Add a directory of ROMs",
			"About and Help",
			"Exit"
		};
		private bool _mainmenu() {
			Program.printtagline();
			int selection;
			string menulist = null;
			for (int i = _mainmenustrings.GetLowerBound(0); i <= _mainmenustrings.GetUpperBound(0); i++)
				menulist += string.Format("\n{0})\t{1}", i + 1, _mainmenustrings[i]);
			while (true) {
				try {
					selection = int.Parse(_prompt("\nEnter the number and press enter.\n" + menulist));
					string newromdir;
					switch (selection-1) {
						case (int)_mainmenuoptions.adddir:
							newromdir = _adddirdlg();
							string newromdirset = _prompt("Would you like to set this directory as the default for a movie type? This will make it the default directory in the file selection dialog when if the ROM is not located in the database. If so, please enter the extention of the movie format (fcm, vbm, etc) or leave blank and press enter to skip.");
							if (newromdirset != "") {
								_settings["defaultdirs", newromdirset] = newromdir;
								Console.WriteLine("Default directory has been set.");
							}
							break;
						case (int)_mainmenuoptions.about:
							Program.printtagline();
							#region too much text
							Console.WriteLine(@"
This software is designed to assist in the viewing of emulator key press
recordings. Once provided with a list of directories, the software creates a
database of hashes of the roms stored there-in. When a movie file is run
through the commandline of this software, it will attempt to match the hash
stored in the movie player with the local file on your computer, then run
the emulator with the movie and rom.

This software can either be configured through it's main menu, viewable by
running the application with no commandline, or via it's ini file. The
settings are stored in the settings.ini file located in the same program
directory. The file is layed out as follows:

[config]
dirs=C:\directory 1\;C:\directory 2;D:\ROMs;D:\etc\etc\etc
fcm=C:\directory and\filename to\fcm playing\emulator.exe

[defaultdirs]
fcm=D:\ROMs

config->dirs lists the directories you wish to be stored in the database. The
directories are not case sensitive, accept spaces with out using quotes (quotes
should never be used) and are semicolon delminated. directories are not scanned
recursively.

config->fcm stores the path and filename to the emulator used to play fcm video
files. This should be the newist version of FCEU-Rerecord. Other emulators are
configured the same way.

defaultdirs->fcm contains the directory to default the file selection dialog to
if the rom is not found in the database. This is a per movie format config, and
is fully optional.

To associate this software with a movie format, double click on it in Explorer.
Then select this program from a list of software. You will need to use the
'browse' button for this. Alterntively, it can be run using a commandline
interface, or manualy associated. The program treats the full commandline as
the movie's file and path name, and ignores quotes.");
							#endregion
							break;
						case (int)_mainmenuoptions.exit:
							return true;
							break;
						default:
							Console.WriteLine("Invalid Selection.");
							break;
					}
				} catch (FormatException) {
					Console.WriteLine("Invalid selection.");
				}
			}
		}
		public Program(string[] args) {
			string moviefile = null;
			string emufile = null;

			#region ini
			string inifile = _getprogdir() + @"\config.ini";
			_settings = new INI(inifile);
			string dirs = _settings["config", "dirs"];
			if ((dirs == null) || (dirs.Trim() == "")) {
				Console.WriteLine("Please select your rom dir...\n");
				if ((dirs = _adddirdlg()) == null) {
					fatal("'" + inifile + @"' not found or missing data. Please create and list your rom directories semicolon deliminated as follows:
					
[config]
dirs=c:\my roms\;c:\my other roms\;c:\etc\etc\etc\");
				} else {
					Console.WriteLine("Please note, that you can add new directories by modifying your '" + inifile + @"' configuration file. The dirs key should contain directories which are semicolon deliminated, for example:

[config]
dirs=c:\my roms\;c:\my other roms\;c:\etc\etc\etc\

Press any key to continue.");
					Console.ReadKey();
				}
			}
			#endregion

			#region args the lazy way
			moviefile = String.Join(" ", args).Replace('"', ' ').Trim();	//Hack.
			if ((moviefile == null) || (moviefile == "")) {
				_mainmenu();
				Environment.Exit(0);
			}
			if (!File.Exists(moviefile)) {
				fatal("Movie file '" + moviefile + @"' does not exist.

Usage:   program.exe movie file
Example: program.exe C:\blah\Dragon Warrior (U).exe");
			}
			#endregion

			db = new database(_getprogdir() + @"\roms." + _dbversion.ToString() + ".db");
			emuMovie movie = new emuMovie(moviefile);
			emufile = _settings["config", movie.FileExt];
			if (!File.Exists(emufile)) {		
				emufile = _filedlg(_promptdlgtype.emu);
				if (emufile == null)
					fatal("The movie can not be played with out an emulator being set. Please re-run the program and select one or please set the value through your ini as follows:\n\n[config]\n" + movie.FileExt + @"=C:\path\and\filename.exe");
				_settings["config", movie.FileExt] = emufile;
			}
			ArrayList romfiles = movie.ROM();
			string romfile = "";
			if (romfiles == null) {
				if (_promptyesno("Unable to locate the ROM in the database. Would you like to update? If not, a file selection dialog will show."))
					_updatedb(dirs);
			}
			romfiles = movie.ROM();
			if (romfiles == null) {
				romfile = _filedlg(_promptdlgtype.rom, movie.FileExt);
			} else if (romfiles.Count == 1) {
				romfile = romfiles[0].ToString();
			} else {
				for (int i = 0; i < romfiles.Count; i++)
					Console.WriteLine("{0})\t{1}", i+1, romfiles[i]);
				int selection = 0;
				try {
					selection = int.Parse(_prompt("Please enter the number for the rom you wish to use."));
				} catch (FormatException) {
					Program.fatal("Invalid selection.");
				}
				if ((selection > 0) && (selection <= romfiles.Count)) {
					romfile = romfiles[selection - 1].ToString();
				} else {
					Program.fatal("Invalid selection.");
				}
			}
			if (!File.Exists(romfile)) {
				if (romfiles == null) {
					db.Remove(romfile);
					fatal("The ROM file with hash of " + movie.Hash + " was identified in the database, but the file '" + romfile + "' associated with it was not found. The dead entry has been removed from the database.");
				}
			}
			_emuExec(emufile, movie, romfile);
			db.Close();
		}
		public static string bytejoin(byte[] bytes) {	//This should be done differntly.
			string ret = "";
			for (int i = 0; i <= bytes.GetUpperBound(0); i++)
				ret += bytes[i].ToString("X").PadRight(2, '0');
			return ret;
		}
		public static void printtagline() {
			Console.WriteLine("Emulator Movie Frontend " + _version.ToString() + "\nJoe Lennox - JLennox" + "@gmail.com - http://www.red-stars.net");
		}
		public static void fatal(string error) {
			if (Program.db != null)
				Program.db.Close();
			Program.printtagline();
			Console.WriteLine("\nError:\n{0}\n\nPress any key to exit.", error);
			Console.ReadKey();
			Environment.Exit(1);
		}
		private string _prompt(string query, string prompt) {
			Console.WriteLine(query);
			Console.Write("\n{0}", prompt);
			return Console.ReadLine();
		}
		private string _prompt(string query) { return _prompt(query, "?> "); }
		private bool _promptyesno(string query, string prompt) {
			string ret = _prompt(query + " [yes/no]", prompt).ToLower();
			if ((ret == "y") || (ret == "yes"))
				return true;
			return false;
		}
		private bool _promptyesno(string query) { return _promptyesno(query, "?> "); }
		private string _filedlg(_promptdlgtype type, string def) {
			OpenFileDialog romDlg = new OpenFileDialog();
			switch (type) {
				case _promptdlgtype.rom:
					string dir = _settings["defaultdirs", def];
					if (!Directory.Exists(dir))
						dir = "";
					romDlg.InitialDirectory = dir;
					romDlg.Title = "Please select the ROM to use";
					romDlg.Filter = "All ROM Files (*.nes;*.gb;*.gbc;*.gba)|*.nes;*.gb;*.gbc;*.gba|NES ROMs (*.nes)|*.nes|Gameboy ROMs (*.gb;*.gbc;*.gba)|*.gb;*.gbc;*.gba|All Files (*.*)|*.*";
					break;
				case _promptdlgtype.emu:
					romDlg.Title = "Please select the emulator to use";
					romDlg.Filter = "Executables (*.exe)|*.exe";
					break;
				case _promptdlgtype.dir:	//sorry, seems can't be done.
				default:
					return null;
			}
			if (romDlg.ShowDialog() == DialogResult.OK)
				return romDlg.FileName;
			else
				return null;
		}
		private string _filedlg(_promptdlgtype type) { return _filedlg(type, null); }
		private string _adddirdlg() {
			string dirs = _settings["config", "dirs"];
			FolderBrowserDialog romDlg = new FolderBrowserDialog();
			if (romDlg.ShowDialog() == DialogResult.OK) {
				_settings["config", "dirs"] = romDlg.SelectedPath + (dirs != "" ? ";" : "") + dirs;
				return romDlg.SelectedPath;
			}  else
				return null;
		}
		private void _updatedb(string dirs) {
			string[] adirs = dirs.Split(';');
			Program.db.startTransaction("update");
			int newFiles = 0;
			foreach (string dir in adirs) {
				if (!Directory.Exists(dir))
					Console.WriteLine("Warning: {0} does not exist!", dir);
				else {
					Console.WriteLine("Updating {0}...", dir);
					FileIOPermission permission = new FileIOPermission(FileIOPermissionAccess.Read, dir); 
					string[] files = Directory.GetFiles(dir);
					String insFile=null;
					String crc=null;
					int size=0;
					foreach (string file in files) {
						if (Program.db.findSum(file) == null) {
							Stream streamRom = null;
							ZipEntry curzEntry = null;
							emuRom rom = null;
							
							if (file.EndsWith("zip", true, System.Globalization.CultureInfo.CurrentCulture)) {
								try {
									ZipFile zFile = new ZipFile(file);
									foreach (ZipEntry zEntry in zFile) {
										if (curzEntry == null)
											curzEntry = zEntry;
										if (zEntry.Size > curzEntry.Size)
											curzEntry = zEntry;
									}
									streamRom = zFile.GetInputStream(curzEntry);
									size = (int)curzEntry.Size;
									insFile = curzEntry.Name;
									crc = curzEntry.Crc.ToString("X");
								} catch (ZipException e) {
									Console.WriteLine("Error Opening file {0}, skipping...", file);
									streamRom = null;
								}
							} else {
								streamRom = File.OpenRead(file);
								size = (int)streamRom.Length;	//This is technically error prone.
								insFile = file;
								crc = null;
							}
							try {
								if (streamRom != null) {
									rom = new emuRom(streamRom, insFile, size, crc);
									if (rom.Hash != null) {
										newFiles++;
										Console.WriteLine("File:\t{0}\nHash:\t{1}\nSize:\t{2}\n", file, rom.Hash, size);
										Program.db.Write(file, rom.Hash);
									}
								}
							} catch (ICSharpCode.SharpZipLib.ZipException e) {
								Console.WriteLine("Zip Exception:\n{0}\nSkipping file...", e);
							} catch (System.ArgumentException e) {
								Console.WriteLine("Argument Exception:\n{0}\nSkipping file...", e);
							}

						}
					}
				}
			}
			Program.db.endTransaction("update");
			Console.WriteLine("Finished update. " + newFiles + " new files added to the database.");
		}
		private void _emuExec(string emu, emuMovie movie, string rom) {
			string args = String.Format(movie.CommandLine, movie.FileName, rom);
			Process procemu = new Process();
			procemu.StartInfo.FileName = emu;
			procemu.StartInfo.Arguments = args;
			procemu.Start();
		}
		//code from http://www.yoda.arachsys.com/csharp/readbinary.html
		public static void ReadStream(Stream stream, byte[] data) {
			int offset = 0;
			int remaining = data.Length;
			while (remaining > 0) {
				int read = stream.Read(data, offset, remaining);
				if (read <= 0)
					throw new EndOfStreamException
						(String.Format("End of stream reached with {0} bytes left to read", remaining));
				remaining -= read;
				offset += read;
			}
		}
		public static string getFileExt(string filename) {
			int per;
			if ((per = filename.LastIndexOf('.')) == 0)
				return null;
			return filename.Substring(per+1);
		}
	}

	class emuRom {
		public string Hash=null;
		private Stream _fs;
		private _hashtypes _hashtype;
		private int _hashstart;
		private int _hashlength;
		private string _crc32;
		private enum _hashtypes {
			crc32, crc16, md5, stored, none
		}
		public emuRom(Stream fs, string filename, int size, string crc32) {
			_crc32 = crc32;
			_settype(filename);
			_sethash(fs, size);
		}
		public void _settype(string filename) {
			switch (Program.getFileExt(filename)) {
				case "nes":
					_hashtype = _hashtypes.md5;
					_hashstart = 16;
					break;
				case "gb":
				case "gbc":
					_hashtype = _hashtypes.stored;
					_hashstart = 0x134;
					_hashlength = 12;
					break;
				case "gba":
					_hashtype = _hashtypes.stored;
					_hashstart = 0xA0;
					_hashlength = 12;
					break;
				case "z64":
				case "n64":
					_hashtype = _hashtypes.stored;
					_hashstart = 0x10;
					_hashlength = 4;
					break;
				default:
					_hashtype = _hashtypes.none;
					break;
			}
		}
		private void _sethash(Stream fs, int size) {
			byte[] data;
			switch (_hashtype) {
				case _hashtypes.md5:
					data = new byte[size];
					Program.ReadStream(fs, data);
					Hash = _getmd5(data);
					break;
				case _hashtypes.crc32:
					data = new byte[size];
					Program.ReadStream(fs, data);
					Hash = _getcrc32(data);
					break;
				case _hashtypes.stored:
					Hash = _getstored(fs);
					break;
				case _hashtypes.none:
				default:
					Hash = null;
					break;
			}
		}
		private string _getstored(Stream fs) {
			byte[] data = new byte[_hashlength];
			for (int i = 1; i <= _hashstart; i++)	//Compressed streams don't support Seek, but do not set CanSeek to false.
				fs.ReadByte();
			fs.Read(data, 0, _hashlength);
			return Program.bytejoin(data);
		}
		private string _getcrc32(byte[] data) {
			if (_crc32 != null)
				return _crc32;
			CodeProject.CRC32 crc32 = (CodeProject.CRC32)CodeProject.CRC32.Create("crc32");
			return Program.bytejoin(crc32.ComputeHash(data));
		}
		private string _getmd5(byte[] data) {
			MD5 md5 = new MD5CryptoServiceProvider();
			byte[] result = md5.ComputeHash(data, _hashstart, data.Length-_hashstart);
			return Program.bytejoin(result);
		}
	}

	class emuMovie {
		#region Members
		public string Hash;
		public string FileExt;
		public string FileName;
		public string CommandLine;
		private FileStream _fs;
		private _movietypes _type;
		private int _hashstart, _hashlength;
		private bool _unsupported = false;
		#endregion
		private enum _movietypes {
			fcm, vbm, m64, smv
		}
		public emuMovie(string filename) {
			FileIOPermission permission = new FileIOPermission(FileIOPermissionAccess.Read, Path.GetFullPath(filename)); 
			try { 
				_fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
			} catch (UnauthorizedAccessException e) {
				Program.fatal("Unauthorized access exception while trying to open the movie file for read access.\n" + e);
			} catch (ArgumentException e) {
				Program.fatal("Argument exception while opening movie file.\n" + e);
			}
			FileExt = Program.getFileExt(filename);
			FileName = filename;
			_setmovietype();
			Hash = _readhash();
			_close();
			if (_unsupported)
				Program.fatal("Even though there is internal support for this emulator movie format, it remains unsupported. The problem may lay in the emulator not supporting commandline movie playing, or the internal support is incomplete.");
		}

		public ArrayList ROM() {
			return Program.db.findRom(Hash);
		}
		private void _close() { _fs.Close(); }
		private void _setmovietype() {
			switch (FileExt) {
				case "fcm":
					_type = _movietypes.fcm;
					_hashstart = 0x20;
					_hashlength = 16;
					CommandLine = "-playmovie \"{0}\" \"{1}\"";
					break;
				case "vbm":
					_type = _movietypes.vbm;
					_hashstart = 0x24;
					_hashlength = 12;
					CommandLine = "-rom \"{1}\" -play \"{0}\" 1";
					break;
				case "m64":
					_unsupported = true;
					_type = _movietypes.m64;
					_hashstart = 0xE4;
					_hashlength = 4;
					break;
				case "smv":
					_unsupported = true;
					_type = _movietypes.smv;
					_hashstart = 0x23;
					_hashlength = 4;
					break;
				default:
					Program.fatal("Unknown movie format.");
					break;
			}
		}
		private string _readhash() {
			if (_type == _movietypes.smv) {
				byte[] hasextra = new byte[1];
				_fs.Seek(0x16, SeekOrigin.Begin);
				_fs.Read(hasextra, 0, 1);
				if ((hasextra[0] & 0x80) == 0)
					Program.fatal("This smv movie is not supported because it does not contain the optional ROM information fields.");
			}
			byte[] storedHash = new byte[_hashlength];
			_fs.Seek(_hashstart, SeekOrigin.Begin);
			_fs.Read(storedHash, 0, _hashlength);
			return Program.bytejoin(storedHash);
		}
	}

	class database {
		private SQLiteClient _sql;
		public database(string filename) {
			_sql = new SQLiteClient(filename);
			try {	//"IF NOT EXIST" DOES NOT EXIST in the SQLite version for C#.
				_sql.Execute("CREATE TABLE roms (id INTEGER AUTOINCREMENT PRIMARY KEY, file TEXT, sum TEXT)");
			} catch (SQLiteException e) { /* Oh well. */ }
		}
		public void Close() { if (_sql != null) { _sql.Close(); } }
		public ArrayList findRom(string sum) { return _find("file", "sum", sum); }
		public ArrayList findSum(string file) { return _find("sum", "file", file); }
		public void Write(string file, string sum) { if (sum != null) { _write(file, sum); } }
		public void Remove(string file) {
			_exec(string.Format("DELETE FROM roms WHERE file={0}", _quote(file)));
		}
		public void startTransaction(string name) {
			_exec("BEGIN TRANSACTION " + _quote(name));
		}
		public void endTransaction(string name) {
			_exec("END TRANSACTION " + _quote(name));
		}
		private ArrayList _find(string retcol, string findcol, string val) {
			SQLiteResultSet find;
			try {
				find = _sql.Execute(String.Format("SELECT {0} FROM roms WHERE {1}={2}", retcol, findcol, _quote(val)));
			} catch (SQLiteException e) { return null; }
			if (find.Rows.Count == 0)
				return null;
			ArrayList ret = new ArrayList() ;
			for (int i = 0; i < find.Rows.Count; i++)
				ret.Add(((ArrayList)find.Rows[i])[0]);
			return ret;
		}
		private void _write(string file, string sum) {
			_exec(String.Format("INSERT INTO roms VALUES (NULL, {0}, {1})", _quote(file), _quote(sum)));
		}
		private void _exec(string cmd) {
			try {
				_sql.Execute(cmd);
			}  catch (SQLiteException e) {
				Console.WriteLine(String.Format("SQL ERROR:\n{0}\n{1}", cmd, e));
			}
		}
		private string _quote(string input) {	//Lets hope this is good enough.
			if (input == null)
				return "''";
			return "'" + input.Replace("'", "''").Trim() + "'";
		}
	}
	public class INI {
		public string path;
		public INI(string path) { this.path = path; }
		public string this[string section, string key] {
			get {
				StringBuilder tmp = new StringBuilder(255);
				NTKernel.GetPrivateProfileString(section, key, "", tmp, 255, path);
				return tmp.ToString();
			}
			set { NTKernel.WritePrivateProfileString(section, key, value, this.path); }
		}
	}
	public class NTKernel {
		#region INI
		[DllImport("kernel32")]
		public static extern long WritePrivateProfileString(string section,
		string key, string val, string filePath);
		[DllImport("kernel32")]
		public static extern int GetPrivateProfileString(string section,
		string key, string def, StringBuilder retVal, int size, string filePath);
		#endregion
	} 
}
