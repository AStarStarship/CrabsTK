import os
import shutil

renames = """
	deleted:    Code/CodeModule.inl
	deleted:    Code/CommentStripper.inl
	deleted:    Code/Include.inl
	renamed:    Code/_Seams.inl -> Code/_Seams.h
	deleted:    Code/stb_c_lexer.inl
	deleted:    Code/stb_leakcheck.inl
	deleted:    Forms/Document.inl
	renamed:    GUI/_Package.inl -> GUI/_Package.h
	renamed:    IMUL/_Seams.inl -> IMUL/_Seams.h
	deleted:    Image/Image.inl
	deleted:    Image/SOIL2.inl
	renamed:    Image/_Package.inl -> Image/_Package.h
	deleted:    Image/etc1_utils.inl
	deleted:    Image/image_DXT.inl
	deleted:    Image/image_helper.inl
	deleted:    Pro/_Config.inl
	renamed:    Pro/_Debug.inl -> Pro/_Debug.h
	renamed:    Pro/_Library.inl -> Pro/_Library.h
	renamed:    Pro/_Release.inl -> Pro/_Release.h
	renamed:    Pro/_Seams.inl -> Pro/_Seams.h
	renamed:    Pro/_Undef.inl -> Pro/_Undef.h
	deleted:    Touch/_Config.inl
	renamed:    Touch/_Debug.inl -> Touch/_Debug.h
	renamed:    Touch/_Library.inl -> Touch/_Library.h
	renamed:    Touch/_Release.inl -> Touch/_Release.h
	renamed:    Touch/_Seams.inl -> Touch/_Seams.h
	renamed:    Touch/_Undef.inl -> Touch/_Undef.h
	renamed:    Who/_Config.inl -> Who/_Config.h
	renamed:    Who/_Debug.inl -> Who/_Debug.h
	renamed:    Who/_Library.inl -> Who/_Library.h
	renamed:    Who/_Release.inl -> Who/_Release.h
	renamed:    Who/_Seams.inl -> Who/_Seams.h
	renamed:    Who/_Undef.inl -> Who/_Undef.h
	renamed:    _Package.inl -> _Package.h
	renamed:    _Seams/Audio/00.Core.inl -> _Seams/Audio/00.Core.h
	renamed:    _Seams/Code/00.Core.inl -> _Seams/Code/00.Core.h
	renamed:    _Seams/Data/00.Core.inl -> _Seams/Data/00.Core.h
	renamed:    _Seams/Forms/00.Core.inl -> _Seams/Forms/00.Core.h
	renamed:    _Seams/GUI/00.Core.inl -> _Seams/GUI/00.Core.h
	renamed:    _Seams/Image/00.Core.inl -> _Seams/Image/00.Core.h
	deleted:    _Seams/Imul/00.Core.inl
	renamed:    _Seams/Pro/00.Core.inl -> _Seams/Pro/00.Core.h
	renamed:    _Seams/Release.inl -> _Seams/Release.h
	renamed:    _Seams/Touch/00.Core.inl -> _Seams/Touch/00.Core.h
	renamed:    _Seams/Who/00.Core.inl -> _Seams/Who/00.Core.h
	renamed:    _Seams/_Debug.inl -> _Seams/_Debug.h
	renamed:    _Seams/_Release.inl -> _Seams/_Release.h
	deleted:    _Seams/_Seams.inl
	renamed:    _Seams/_Undef.inl -> _Seams/_Undef.h
"""

untracked = """
	Code/CodeModule.hxx
	Code/CommentStripper.hxx
	Code/Include.hxx
	Code/_Seams.hxx
	Code/stb_c_lexer.hxx
	Code/stb_leakcheck.hxx
	Forms/Document.hxx
	GUI/_Package.hxx
	IMUL/_Seams.hxx
	Image/Image.hxx
	Image/SOIL2.hxx
	Image/_Package.hxx
	Image/etc1_utils.hxx
	Image/image_DXT.hxx
	Image/image_helper.hxx
	Pro/_Config.hxx
	Pro/_Debug.hxx
	Pro/_Library.hxx
	Pro/_Release.hxx
	Pro/_Seams.hxx
	Pro/_Undef.hxx
	Touch/_Config.hxx
	Touch/_Debug.hxx
	Touch/_Library.hxx
	Touch/_Release.hxx
	Touch/_Seams.hxx
	Touch/_Undef.hxx
	Who/_Config.hxx
	Who/_Debug.hxx
	Who/_Library.hxx
	Who/_Release.hxx
	Who/_Seams.hxx
	Who/_Undef.hxx
	_Package.hxx
	_Seams/Audio/00.Core.hxx
	_Seams/Code/00.Core.hxx
	_Seams/Data/00.Core.hxx
	_Seams/Forms/00.Core.hxx
	_Seams/GUI/00.Core.hxx
	_Seams/Image/00.Core.hxx
	_Seams/Imul/00.Core.hxx
	_Seams/Pro/00.Core.hxx
	_Seams/Release.hxx
	_Seams/Touch/00.Core.hxx
	_Seams/Who/00.Core.hxx
	_Seams/_Debug.hxx
	_Seams/_Release.hxx
	_Seams/_Seams.hxx
	_Seams/_Undef.hxx
"""

for line in renames.strip().split('\n'):
    line = line.strip()
    if line.startswith('renamed:'):
        parts = line.split('->')
        src = parts[0].replace('renamed:', '').strip()
        dst = parts[1].strip()
        if os.path.exists(src):
            print(f"git mv {src} {dst}")
            os.system(f"git mv {src} {dst}")
    elif line.startswith('deleted:'):
        src = line.replace('deleted:', '').strip()
        # Find if there is a corresponding .hxx file in the untracked list
        basename = os.path.splitext(src)[0]
        dst = basename + '.hxx'
        if f"{dst}" in untracked:
            if os.path.exists(src):
                print(f"git mv {src} {dst}")
                os.system(f"git mv {src} {dst}")
        else:
            print(f"WARNING: No destination found for {src}")

