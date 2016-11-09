function varargout = termopar_gui(varargin)
% TERMOPAR_GUI MATLAB code for termopar_gui.fig
%      TERMOPAR_GUI, by itself, creates a new TERMOPAR_GUI or raises the existing
%      singleton*.
%
%      H = TERMOPAR_GUI returns the handle to a new TERMOPAR_GUI or the handle to
%      the existing singleton*.
%
%      TERMOPAR_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TERMOPAR_GUI.M with the given input arguments.
%
%      TERMOPAR_GUI('Property','Value',...) creates a new TERMOPAR_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before termopar_gui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to termopar_gui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help termopar_gui

% Last Modified by GUIDE v2.5 08-Nov-2016 23:14:07

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @termopar_gui_OpeningFcn, ...
                   'gui_OutputFcn',  @termopar_gui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before termopar_gui is made visible.
function termopar_gui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to termopar_gui (see VARARGIN)

% Choose default command line output for termopar_gui
handles.output = hObject;

serialPorts = instrhwinfo('serial');
nPorts = length(serialPorts.SerialPorts);
set(handles.popupmenuPort, 'String', ...
    [{'Select a port'} ; serialPorts.SerialPorts ]);
set(handles.toggleConnect, 'Value', 0);
value = get(handles.toggleConnect, 'Value')

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes termopar_gui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = termopar_gui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in popupmenuPort.
function popupmenuPort_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenuPort (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenuPort contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenuPort


% --- Executes during object creation, after setting all properties.
function popupmenuPort_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenuPort (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function toggleConnect_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonColorbar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if (get(handles.toggleConnect, 'Value') == 1)
    set(handles.toggleConnect, 'String', 'Desconectar');
    handles.toggleConnect = 0;
    disp('The game is on!');
else
   set(handles.toggleConnect, 'String', 'Conectar');
   set(handles.toggleConnect, 'Value', 1);
end


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over toggleConnect.
function toggleConnect_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to toggleConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if (get(handles.toggleConnect, 'Value') == 1)
    set(handles.toggleConnect, 'String', 'Desconectar');
    handles.toggleConnect = 0;
    disp('The game is on!');
elseif (get(handles.toggleConnect, 'Value') == 0)
   set(handles.toggleConnect, 'String', 'Conectar');
   set(handles.toggleConnect, 'Value', 1);
   disp('*sighs*')
end

