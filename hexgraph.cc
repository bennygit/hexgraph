// Lisenced under the
// The MIT License (MIT)
// 
// Copyright (c) 2013 bennygit @ https://github.com/bennygit/hexgraph
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include <iostream>
#include <fstream>
#include <cinttypes>
#include <string>

#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TAxis.h"

#include "tclap/CmdLine.h"

using namespace std;

int main (int argc, char** argv) {
    string filename;
    long startByte = 0;
    long lastByte = 0;
    
    try {
    TCLAP::CmdLine cmd("This program plots the hexdump of a file in a graph for visual analysis.", ' ', "0.9");
    TCLAP::UnlabeledValueArg<std::string> fileArg( "filename", "filename of the file to be plotted", true, "", "filename");
    cmd.add(fileArg);

    TCLAP::ValueArg<long> startArg("s", "start", "set start byte counted from begin of file "
                                "(last minus start should be even)", false, 0, "long");
	cmd.add(startArg);

    TCLAP::ValueArg<long> lastArg("l", "last", "set last byte counted from end of file "
                                "(last minus start should be even)", false, 0, "long");
	cmd.add(lastArg);

    cmd.parse( argc, argv );
    filename = fileArg.getValue();
    startByte = startArg.getValue();
    lastByte = lastArg.getValue();
    std::cout << startByte << lastByte <<std::endl;
    }
    catch(TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(-1);
    }


    // opening file and reading it in as binary in chunks of bytes and
    // converting it to 16 bit words.
    ifstream file (filename.c_str(), ios::in | ios::binary);
    file.seekg(startByte, ios::beg);
    long begin = file.tellg();
    file.seekg ((- lastByte), ios::end);
    long end = file.tellg();
    file.seekg(startByte, ios::beg);

    long sizeInBytes = end - begin;
    long size = sizeInBytes / 2;
    uint16_t *data_ui = new uint16_t[size];

    file.read((char*)data_ui, sizeInBytes);
    file.close();
    cout << "Read in size is: " << sizeInBytes << " bytes" << endl;


    // do the plotting
    uint16_t *numEntries_ui = new uint16_t[size];
    for(uint16_t i = 0; i < size; ++i) {
        numEntries_ui[i] = i;
    }

    float *numEntries = new float[size];
    float *data = new float[size];
    std::copy(numEntries_ui, numEntries_ui + size, numEntries);
    std::copy(data_ui, data_ui + size, data);

    TApplication app("HexGraph", &argc, argv);
    TCanvas *canvas = new TCanvas("c", "c", 800, 600);
    TGraph *graph = new TGraph(size, numEntries, data);
    graph->SetTitle(filename.c_str());
    graph->GetXaxis()->SetTitle("# of 16 bit words");
    graph->GetYaxis()->SetTitle("value stored in word");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();
    graph->Draw("AL");
    

    app.Run();
    
    delete canvas;
    delete graph;
    return 0;
}

