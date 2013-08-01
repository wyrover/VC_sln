Function Include(file)
	ExecuteGlobal CreateObject("Scripting.FileSystemObject").OpenTextFile(file,1).ReadAll
End Function

'//---------------------------------------------------------------
'// 程序入口
'//---------------------------------------------------------------

Include "common.vbs"

Dim WinTitle
Dim PROJECT_DIR
Dim PROJECT_NAME

WinTitle = "创建VC2005 WTL工程"
 
PROJECT_DIR = InputBox("项目目录", WinTitle, left(Wscript.ScriptFullName,len(Wscript.ScriptFullName)-len(Wscript.ScriptName)))

If PROJECT_DIR = False Then
	Wscript.Quit
Else
	PROJECT_DIR = DisposePath(PROJECT_DIR)
End If

PROJECT_NAME = InputBox("项目名称", WinTitle)

If PROJECT_NAME = False Then
	Wscript.Quit		
End If



Dim objShell
Set objShell = wscript.createObject("wscript.shell")
iReturn = objShell.Run("7z x ""VS2005WTL2.7z"" -y -o""" & PROJECT_DIR  & """", 1, TRUE)


Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\resource.h", "VS2005WTL2", PROJECT_NAME, 0) 
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\stdafx.cpp", "VS2005WTL2", PROJECT_NAME, 0) 
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.cpp", "VS2005WTL2", PROJECT_NAME, 0)
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.h", "VS2005WTL2", PROJECT_NAME, 0)
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.rc", "VS2005WTL2", PROJECT_NAME, 0)
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.vcproj", "VS2005WTL2", PROJECT_NAME, 0)
Call ReplaceFileContent(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.vcproj", "\{4783B7C6\-FEEE\-4027\-9AF3\-CBEF84DEFA73\}", NewGUID, 0)




' 重命名文件
Call ReFilename(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.cpp", PROJECT_NAME & ".cpp") 
Call ReFilename(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.h", PROJECT_NAME & ".h")
Call ReFilename(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.rc", PROJECT_NAME & ".rc")
Call ReFilename(PROJECT_DIR & "VS2005WTL2\VS2005WTL2.vcproj", PROJECT_NAME & ".vcproj")
Call ReFilename(PROJECT_DIR & "VS2005WTL2\res\VS2005WTL2.ico", PROJECT_NAME & ".ico")

Call ReDir(PROJECT_DIR & "VS2005WTL2", PROJECT_DIR & PROJECT_NAME)


Wscript.Quit
