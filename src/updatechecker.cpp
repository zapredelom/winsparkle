/*
 *  This file is part of WinSparkle (http://winsparkle.org)
 *
 *  Copyright (C) 2009 Vaclav Slavik
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 */

#include "updatechecker.h"
#include "appcast.h"
#include "ui.h"
#include "error.h"
#include "settings.h"
#include "download.h"

namespace winsparkle
{

UpdateChecker::UpdateChecker(): Thread("WinSparkle updates check")
{
}


void UpdateChecker::Run()
{
    // no initialization to do, so signal readiness immediately
    SignalReady();

    try
    {
        const std::string url = Settings::Get().AppcastURL;
        if ( url.empty() )
            throw std::runtime_error("Appcast URL not specified.");

        StringDownloadSink appcast_xml;
        DownloadFile(url, &appcast_xml);

        Appcast appcast;
        appcast.Load(appcast_xml.data);

        // FIXME: really check appcast data...
        OutputDebugStringA("WinSparkle appcast data:\n");
        OutputDebugStringA(("    Version:       " + appcast.Version + "\n").c_str());
        OutputDebugStringA(("    Download:      " + appcast.DownloadURL + "\n").c_str());
        OutputDebugStringA(("    Release notes: " + appcast.ReleaseNotesURL + "\n").c_str());

        UI::NotifyNoUpdates();
    }
    catch ( ... )
    {
        UI::NotifyNoUpdates();
        throw;
    }
}

} // namespace winsparkle
