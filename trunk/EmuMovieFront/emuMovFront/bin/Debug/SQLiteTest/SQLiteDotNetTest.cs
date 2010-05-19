//
// This file is part of SQLite.NET
//
// SQLite.NET is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// SQLite.NET is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SQLite.NET; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// © Copyright 2004 Richard Heyes
//
using System;
using HLib;
using System.Collections;
using System.IO;
using SQLite.NET;

namespace SQLite.NET
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class SQLiteTest
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main(string[] args)
		{
			SQLiteClient db;
			SQLiteResultSet results;

			Console.WriteLine("Basic test app for SQLite.NET. Enter a single line sql query and it will be run against the database.\r\n");
			string dbfile = string.Join(" ", args).Replace('"', ' ').Trim();
			if (!File.Exists(dbfile)) {
				Console.WriteLine("{0} was unabled to be opened.", dbfile);
				Environment.Exit(1);
			}
			Console.WriteLine("Opening database '{0}'...\r\n", dbfile);
			
			try {
				// Open database
				db = new SQLiteClient(dbfile);

			} catch (SQLiteException e) {
				Console.WriteLine("Fatal error: {0}", e.Message);
				Console.ReadLine();
				return;
			}

			Console.WriteLine("Available tables:");

			ArrayList tables = db.GetColumn("SELECT name FROM sqlite_master WHERE type = 'table'");

			foreach (string tableName in tables) {
				Console.WriteLine("\t" + tableName);
			}

			// Main loop
			while (true) {
				Console.Write("SQL> ");
				string input = Console.ReadLine();

				if (input == null || input.Trim() == "") {
					continue;
				}

				if (input == ".quit") {
					return;
				}

				try {
					results = db.Execute(input);

					ConsoleTable table = new ConsoleTable();
					table.SetHeaders(results.ColumnNames);

					// Loop through the results and display them
					foreach (ArrayList arr in results.Rows) {
						table.AppendRow(arr);
					}

					while (results.IsMoreData) {
						Hashtable foo = results.GetRowHash();
						foo.GetType();
					}

					Console.WriteLine(table.ToString());

				} catch (SQLiteException e) {
					Console.WriteLine(e.Message);
				}
			}
		}
	}
}
