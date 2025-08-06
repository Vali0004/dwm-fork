/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 1;               /* border pixel of windows */
static const unsigned int snap           = 32;              /* snap pixel */
static const unsigned int gappih         = 3;               /* horiz inner gap between windows */
static const unsigned int gappiv         = 3;               /* vert inner gap between windows */
static const unsigned int gappoh         = 5;               /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 5;               /* vert outer gap between windows and screen edge */
static const unsigned int systraypinning = 0;               /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;               /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;               /* systray spacing */
static unsigned int vertpad              = 3;               /* vertical padding of bar */
static unsigned int sidepad              = 0;               /* horizontal padding of bar */
static const int systraypinningfailfirst = 1;               /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;               /* 0 means no systray */
static const int smartgaps               = 1;               /* 1 means no outer gap when there is only one window */
static const int showbar                 = 1;               /* 0 means no bar */
static const int topbar                  = 1;               /* 0 means bottom bar */
static const int usealtbar               = 0;               /* 1 means use non-dwm status bar */
static const char *altbarclass           = "Polybar";       /* Alternate bar class name */
static const char *altbarcmd             = "";              /* Alternate bar launch command */
static const char *fonts[]               = { "monospace:size=10" };
static const char dmenufont[]            = "monospace:size=10";
static const char col_gray1[]            = "#0F0F0F";
static const char col_gray2[]            = "#3B3B3B";
static const char col_gray3[]            = "#bbbbbb";
static const char col_gray4[]            = "#eeeeee";
static const char col_yellow[]           = "#fffc43";
static const char col_orange[]           = "#642cff";
static const unsigned int baralpha       = 0xd0;
static const unsigned int borderalpha    = OPAQUE;
static const char *colors[][3] = {
    /*               fg          bg         border   */
    [SchemeNorm] = { col_gray3,  col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray3,  col_gray1, col_orange },
	[SchemeTag]  = { col_gray3,  col_orange, col_orange },
	[SchemeUrg]  = { col_gray3,  col_yellow, col_gray1 },
};
static const unsigned int alphas[][3] = {
    /*               fg      bg        border      */
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeTag]  = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* appicons */
/* NOTE: set to 0 to set to default (whitespace) */
static char outer_separator_beg      = '[';
static char outer_separator_end      = ']';
static char inner_separator          = ' ';
static unsigned truncate_icons_after = 2; /* will default to 1, that is the min */
static char truncate_symbol[]         = "...";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance          title                    tags mask     isfloating   monitor   appicon*/
    { "gnome-calculator",  NULL,             NULL,                    0,            1,           -1,        ""  },
	{ "discord",           "discord",        "Discord Updater",       0,            1,           -1,        ""  },
	{ "Alacritty",         "Alacritty",      NULL,                    0,            0,           -1,        ""  },
	{ "Alacritty",         "Alacritty",      "vim",                   0,            0,           -1,        ""  },
	{ "discord",           "discord",        NULL,                    0,            0,           -1,        ""  },
	{ "Cider",             "cider",          NULL,                    0,            0,           -1,        "󰝚"  },
	{ "Code",              "code",           NULL,                    0,            0,           -1,        "󰨞"  },
	{ "Google-chrome",     "google-chrome",  NULL,                    0,            0,           -1,        ""  },
	{ "mpv",               "mpvk",           NULL,                    0,            0,           -1,        ""  },
	{ "Spotify",           "spotify",        NULL,                    0,            0,           -1,        "󰓇"  },
	{ "steam",             "steamwebhelper", NULL,                    0,            0,           -1,        ""  },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      = { "dmenu_run_desktop", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]       = { "alacritty", NULL };
static const char *browsercmd[]    = { "google-chrome-stable", NULL };
static const char *clipcmd[]       = { "clipmenu-paste", NULL };
static const char *screenshotcmd[] = { "flameshot_fuckk_lol", NULL };
static const char *flameshotcmd[]  = { "flameshot", "gui", "--accept-on-select", "--clipboard", NULL };
static const char *lowercmd[]      = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *raisecmd[]      = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *mutecmd[]       = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *playpausecmd[]  = { "playerctl", "play-pause", NULL };
static const char *nextcmd[]       = { "playerctl", "next", NULL };
static const char *prevcmd[]       = { "playerctl", "previous", NULL };

#include <X11/XF86keysym.h>
#include "exitdwm.c"
static const Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY,                       XK_d,                     spawn,          {.v = dmenucmd} },
	{ MODKEY,                       XK_Return,                spawn,          {.v = termcmd} },
	{ MODKEY|ShiftMask,             XK_Return,                spawn,          {.v = browsercmd} },
	{ MODKEY,                       XK_v,                     spawn,          {.v = clipcmd} },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_Left,                  movestack,      {.i = -1} },
	{ MODKEY|ShiftMask,             XK_Right,                 movestack,      {.i = +1} },
	{ MODKEY,                       XK_Left,                  focusstack,     {.i = +1} },
	{ MODKEY,                       XK_Right,                 focusstack,     {.i = -1} },
	{ MODKEY,                       XK_i,                     incnmaster,     {.i = +1} },
	{ MODKEY,                       XK_p,                     incnmaster,     {.i = -1} },
	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,                     setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,                     setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,                     setcfact,       {.f =  0.00} },
	{ Mod1Mask,                     XK_Tab,                   previewallwin,  {0} },
	{ MODKEY,                       XK_q,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_n,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,                     setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_g,                     setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,                 setlayout,      {0} },
	{ MODKEY,                       XK_f,                     togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,                     togglefullscr,  {0} },
	{ MODKEY|Mod1Mask,              XK_u,                     incrgaps,       {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,                     incrgaps,       {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_i,                     incrigaps,      {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,                     incrigaps,      {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_o,                     incrogaps,      {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,                     incrogaps,      {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_6,                     incrihgaps,     {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,                     incrihgaps,     {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_7,                     incrivgaps,     {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,                     incrivgaps,     {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_8,                     incrohgaps,     {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,                     incrohgaps,     {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_9,                     incrovgaps,     {.i = +1} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,                     incrovgaps,     {.i = -1} },
	{ MODKEY|Mod1Mask,              XK_0,                     togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,                     defaultgaps,    {0} },
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0} },
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0} },
	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1} },
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1} },
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1} },
	{ MODKEY|ShiftMask,             XK_period,                tagmon,         {.i = +1} },
	{ 0,  			                XF86XK_AudioLowerVolume,  spawn,          {.v = lowercmd} },
	{ 0,  			                XF86XK_AudioRaiseVolume,  spawn,          {.v = raisecmd} },
	{ 0,  			                XF86XK_AudioMute,         spawn,          {.v = mutecmd} },
	{ 0,  			                XF86XK_AudioPlay,         spawn,          {.v = playpausecmd} },
	{ 0,  			                XF86XK_AudioPause,        spawn,          {.v = playpausecmd} },
	{ 0,  			                XF86XK_AudioNext,         spawn,          {.v = nextcmd} },
	{ 0,  			                XF86XK_AudioPrev,         spawn,          {.v = prevcmd} },
	TAGKEYS(                        XK_1,                                      0)
	TAGKEYS(                        XK_2,                                      1)
	TAGKEYS(                        XK_3,                                      2)
	TAGKEYS(                        XK_4,                                      3)
	TAGKEYS(                        XK_5,                                      4)
	TAGKEYS(                        XK_6,                                      5)
	TAGKEYS(                        XK_7,                                      6)
	TAGKEYS(                        XK_8,                                      7)
	TAGKEYS(                        XK_9,                                      8)
	{ MODKEY|ShiftMask,             XK_e,                      exitdwm,       {0} },
	{ MODKEY|ShiftMask,             XK_r,                      quit,          {1} },
	{ MODKEY,                       XK_o,                      winview,       {0} },
	{ 0,                            XK_Print,                  spawn,         {.v = screenshotcmd} },
	{ ControlMask,                  XK_Print,                  spawn,         {.v = flameshotcmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}                ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}                ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}                ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}                ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}                ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}                ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}                ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}                ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}                ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}                ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}                ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}               ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}                 ),
  IPCCOMMAND(  tagclient,           2,      {ARG_TYPE_UINT, ARG_TYPE_UINT} ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}                )
};
