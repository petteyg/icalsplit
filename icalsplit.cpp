/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

void usage(char **);

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv);
    }
    ifstream fin;
    ofstream fout;
    string infile = argv[1];
    cout << "Opening " << infile << "..." << endl;
    fin.open(infile.c_str());
    stringstream header(stringstream::in|stringstream::out|stringstream::app);
    stringstream events(stringstream::in|stringstream::out|stringstream::app);
    string line;
    bool top = true;
    int count = 0;
    int lines = 0;
    int headerlines = 0;
    getline(fin, line);
    while (fin)
    {
        if (top)
        {
            header << line << endl;
            if (line.substr(0,13).compare("END:VTIMEZONE") == 0)
            {
                top = false;
                cout << "Header done." << endl;
            }
            line = "";
        }
        else
        {
            events << line << endl;
            lines++;
            if (lines > 36000 && line.substr(0,10).compare("END:VEVENT") == 0)
            {
                count++;
                lines = 0;
                stringstream outfile;
                outfile << "calpart_" << count << ".ics\0";
                cout << "Writing " << outfile.str() << endl;
                fout.open(outfile.str().c_str());
                fout << header.str() << endl;
                fout << events.str() << endl;
                fout << "END:VCALENDAR" << endl;
                fout.close();
                events.str("");
            }
        }
        getline(fin, line);
    }
    count++;
    lines = 0;
    stringstream outfile;
    outfile << "calpart_" << count << ".ics\0";
    cout << "Writing " << outfile.str() << "..." << endl;
    fout.open(outfile.str().c_str());
    fout << header.str() << endl;
    fout << events.str() << endl;
    fout.close();
    events.str("");
    events.flush();
    return 0;
}

void usage(char *argv[])
{
    cout << "Usage: " << endl;
    cout << argv[0] << " <filename>" << endl;
    exit(0);
}
