#pragma once
#include <windows.h>
struct maze {
	COORD		 size;
	COORD		 entrance;
	COORD		 exit;
	wchar_t *data;
};
constexpr val wall_char			 = L'■';
constexpr val path_char			 = L'□';
constexpr val entrance_char	 = L'〓';
constexpr val exit_char			 = L'〓';
constexpr val current_char	 = L'㊣';
constexpr val footprint_char = L'☒';
constexpr val left_arrow		 = L'←';
constexpr val right_arrow		 = L'→';
constexpr val up_arrow			 = L'↑';
constexpr val down_arrow		 = L'↓';