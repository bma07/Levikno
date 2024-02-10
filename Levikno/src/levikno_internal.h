#ifndef HG_LEVIKNO_INTERNAL_H
#define HG_LEVIKNO_INTERNAL_H

#include "levikno/levikno.h"


namespace lvn
{
	LvnContext* getContext();

	const char* getDateMonthName();
	const char* getDateMonthNameShort();
	const char* getDateWeekDayName();
	const char* getDateWeekDayNameShort();
	const char* getDateTimeMeridiem();
	const char* getDateTimeMeridiemLower();

	LvnString	getDateTimeHHMMSS();
	LvnString	getDateTime12HHMMSS();
	LvnString	getDateYearStr();
	LvnString	getDateYear02dStr();
	LvnString	getDateMonthNumStr();
	LvnString	getDateDayNumStr();
	LvnString	getDateHourNumStr();
	LvnString	getDateHour12NumStr();
	LvnString	getDateMinuteNumStr();
	LvnString	getDateSecondNumStr();

	LvnString	getFileSrc(const char* filepath);
	LvnVector<uint8_t> getFileSrcBin(const char* filepath);
}

// ------------------------------------------------------------
// [SECTION]: Core Internal structs
// ------------------------------------------------------------

struct LvnLogger
{
	const char* loggerName;
	LvnLogLevel logLevel;
	const char* logPatternFormat;
	LvnLogPattern* pLogPatterns;
	uint32_t logPatternCount;
};

// ------------------------------------------------------------
//  Data Structures
// ------------------------------------------------------------
// - manual implementation to save header compilation time and limit external depedencies
// - These data structures should only mostly be used within the library and not for external use (use std::vector and std::string instead)
// - Important: These Data types should not be included in other sturctures that are allocated in memory
//              unless they are manually freed. Use of malloc and free does not call constructors and 
//              deconstructors of data structures.

struct LvnString
{
	char* m_Str;
	uint32_t m_Size;

	LvnString()
		: m_Str((char*)lvn::memAlloc(0)), m_Size(0) {}
	~LvnString() { lvn::memFree(m_Str); }

	LvnString(const char* strsrc)
	{
		m_Size = strlen(strsrc) + 1;
		m_Str = (char*)lvn::memAlloc(m_Size);
		if (!m_Str) { return; }
		memcpy(m_Str, strsrc, m_Size);
	}

	LvnString(uint32_t strsize)
	{
		m_Size = strsize;
		m_Str = (char*)lvn::memAlloc(m_Size);
		if (!m_Str) { return; }
	}

	LvnString(char* strsrc, uint32_t strsize)
	{
		m_Size = strsize;
		m_Str = (char*)lvn::memAlloc(strsize);
		if (!m_Str) { return; }
		memcpy(m_Str, strsrc, strsize);
	}

	LvnString(const LvnString& lvnstr)
	{
		m_Size = lvnstr.m_Size;
		m_Str = (char*)lvn::memAlloc(m_Size);
		if (!m_Str) { return; }
		memcpy(m_Str, lvnstr.m_Str, m_Size);
	}

	LvnString& operator=(const LvnString& lvnstr)
	{
		m_Size = lvnstr.m_Size;
		m_Str = (char*)lvn::memAlloc(m_Size);
		if (!m_Str) { return *this; }
		memcpy(m_Str, lvnstr.m_Str, m_Size);
		return *this;
	}

	const char* c_str() { return m_Str; }
	uint32_t size() { return m_Size; }

	operator const char* () { return this->m_Str; }
	operator char* () { return this->m_Str; }

	char& operator [](uint32_t i)
	{
		LVN_CORE_ASSERT(i < this->m_Size, "string index out of range");
		return this->m_Str[i];
	}
};


template <typename T>
struct LvnVector
{
	T* m_Data;
	uint32_t m_Size;
	uint32_t m_Capacity;

	LvnVector()
		: m_Data(0), m_Size(0), m_Capacity(0) {}
	~LvnVector() { lvn::memFree(m_Data); }

	LvnVector(uint32_t size)
	{
		m_Size = size;
		m_Capacity = size;
		m_Data = (T*)lvn::memAlloc(size * sizeof(T));
	}

	LvnVector(T* data, uint32_t size)
	{
		m_Size = size;
		m_Capacity = size;
		m_Data = (T*)lvn::memAlloc(size * sizeof(T));
		memcpy(m_Data, data, size * sizeof(T));
	}
	LvnVector(const LvnVector<T>& lvnvec)
	{
		m_Size = lvnvec.m_Size;
		m_Capacity = lvnvec.m_Size;
		m_Data = (T*)lvn::memAlloc(m_Size * sizeof(T));
		memcpy(m_Data, lvnvec.m_Data, m_Size * sizeof(T));
	}
	LvnVector<T>& operator=(const LvnVector<T>& lvnvec)
	{
		resize(lvnvec.m_Size);
		m_Data = (T*)lvn::memAlloc(m_Size * sizeof(T));
		memcpy(m_Data, lvnvec.m_Data, m_Size * sizeof(T));
		return *this;
	}

	T& operator[](uint32_t i)
	{
		LVN_CORE_ASSERT(i < m_Size, "index out of vector size range");
		return m_Data[i];
	}
	const T& operator[](uint32_t i) const
	{
		LVN_CORE_ASSERT(i < m_Size, "index out of vector size range");
		return m_Data[i];
	}

	T*			begin() { return m_Data; }
	const T*	begin() const { return m_Data; }
	T*			end() { return m_Data + m_Size; }
	const T*	end() const { return m_Data + m_Size; }
	T&			front() { LVN_CORE_ASSERT(m_Size > 0, "cannot access index of empty vector"); return m_Data[0]; }
	const T&	front() const { LVN_CORE_ASSERT(m_Size > 0, "cannot access index of empty vector"); return m_Data[0]; }
	T&			back() { LVN_CORE_ASSERT(m_Size > 0, "cannot access index of empty vector"); return m_Data[m_Size - 1]; }
	const T&	back() const { LVN_CORE_ASSERT(m_Size > 0, "cannot access index of empty vector"); return m_Data[m_Size - 1]; }


	bool		empty() { return m_Size == 0; }
	void		clear() { if (m_Data) { memset(m_Data, 0, m_Size * sizeof(T)); } m_Size = 0; }
	void		clear_free() { if (m_Data) { lvn::memFree(m_Data); m_Size = 0; m_Capacity = 0; m_Data = nullptr; } }
	void		erase(const T* it) { LVN_CORE_ASSERT(it >= m_Data && it < m_Data + m_Size, "element not within vector index!"); size_t off = it - m_Data; memmove(m_Data + off, m_Data + off + 1, (m_Size - off - 1) * sizeof(T)); m_Size--; }
	T*			data() { return m_Data; }
	uint32_t	size() { return m_Size; }
	uint32_t	memsize() { return m_Size * sizeof(T); }
	void		resize(uint32_t size) { if (size > m_Capacity) { reserve(size); } m_Size = size; }
	void		reserve(uint32_t size) { if (size <= m_Size) return; T* newsize = (T*)lvn::memAlloc(size * sizeof(T)); memcpy(newsize, m_Data, m_Size * sizeof(T)); lvn::memFree(m_Data); m_Data = newsize; m_Capacity = size; }

	void		push_back(const T& e) { resize(m_Size + 1); memcpy(&m_Data[m_Size - 1], &e, sizeof(T)); }
	void		copy_back(T* data, uint32_t size) { resize(m_Size + size); memcpy(&m_Data[m_Size - size], data, size * sizeof(T)); }
	T*			find(const T& e) { T* begin = m_Data; const T* end = m_Data + m_Size; while (begin < end) { if (*begin == e) break; begin++; } return begin; }
};


// ------------------------------------------------------------
// [SECTION]: Window Internal structs
// ------------------------------------------------------------

/* [Events] */
struct LvnEvent
{
	LvnEventType type;
	int category;
	const char* name;
	bool handled;

	union data
	{
		struct /* Mouse/Window pos & size */
		{
			union
			{
				struct
				{
					int x, y;
				};
				struct
				{
					double xd, yd;
				};
			};
		};
		struct /* key/mouse button codes */
		{
			int code;
			unsigned int ucode;
			bool repeat;
		};
	} data;
};


/* [Window] */
struct LvnWindowData
{                                        /* [Same use with LvnWindowCreateinfo] */
	int width, height;                   /* width and height of window */
	const char* title;				   	 /* title of window */
	int minWidth, minHeight;		   	 /* minimum width and height of window */
	int maxWidth, maxHeight;		   	 /* maximum width and height of window */
	bool fullscreen, resizable, vSync; 	 /* sets window to fullscreen; enables window resizing; vSync controls window framerate */
	LvnWindowIconData* pIcons;		   	 /* icon images used for window/app icon */
	uint32_t iconCount;				   	 /* iconCount is the number of icons in pIcons */
	void (*eventCallBackFn)(LvnEvent*);  /* function ptr used as a callback to get events from this window */
};

/*
  LvnWindow struct is used to create a window on the system
  - Stores window data (eg. width, height, title)
  - window data is changed from the window api being used (eg. glfw)
  Note: LvnWindow is an imcomplete data type; LvnWindow needs to be
		allocated and destroyed with its corresponding functions.
		Use lvn::createWindow() and lvn::destroyWindow()
*/
struct LvnWindow
{
	LvnWindowData data;    /* holds data of window (eg. width, height) */
	void* nativeWindow;    /* used to hold ptr to window api handle (eg. GLFWwindow) */
};

struct LvnWindowContext
{
	LvnWindowApi		windowapi;    /* window api enum */

	void				(*createWindowInfo)(LvnWindow*, LvnWindowCreateInfo*);
	void				(*updateWindow)(LvnWindow*);
	bool				(*windowOpen)(LvnWindow*);
	LvnWindowDimensions	(*getDimensions)(LvnWindow*);
	unsigned int		(*getWindowWidth)(LvnWindow*);
	unsigned int		(*getWindowHeight)(LvnWindow*);
	void				(*setWindowVSync)(LvnWindow*, bool);
	bool				(*getWindowVSync)(LvnWindow*);
	void				(*setWindowContextCurrent)(LvnWindow*);
	void				(*destroyWindow)(LvnWindow*);
};


// ------------------------------------------------------------
// [SECTION]: Graphics Internal structs
// ------------------------------------------------------------

struct LvnGraphicsContext
{
	LvnGraphicsApi graphicsapi;

	void (*getPhysicalDevices)(LvnPhysicalDevice*, uint32_t*);
	bool (*renderInit)(LvnRendererBackends*);

	LvnResult (*createRenderPass)(LvnRenderPass**, LvnRenderPassCreateInfo*);
	LvnResult (*createPipeline)(LvnPipeline**, LvnPipelineCreateInfo*);
	void (*setDefaultPipelineSpecification)(LvnPipelineSpecification*);
	LvnPipelineSpecification (*getDefaultPipelineSpecification)();

	void (*destroyRenderPass)(LvnRenderPass*);
	void (*destroyPipeline)(LvnPipeline*);


	void (*renderClearColor)(const float, const float, const float, const float);
	void (*renderClear)();
	void (*renderDraw)(uint32_t);
	void (*renderDrawIndexed)(uint32_t);
	void (*renderDrawInstanced)(uint32_t, uint32_t, uint32_t);
	void (*renderDrawIndexedInstanced)(uint32_t, uint32_t, uint32_t);
	void (*renderSetStencilReference)(uint32_t);
	void (*renderSetStencilMask)(uint32_t, uint32_t);
	void (*renderBeginNextFrame)();
	void (*renderDrawSubmit)();
	void (*renderBeginRenderPass)();
	void (*renderEndRenderPass)();

};

struct LvnPhysicalDevice
{
	typedef void* LvnPhysicalDeviceHandle;

	LvnPhysicalDeviceInfo info;
	LvnPhysicalDeviceHandle device;
};

struct LvnRenderPass
{
	void* nativeRenderPass;
};

struct LvnPipeline
{
	void* nativePipeline;
	void* nativePipelineLayout;
};

struct LvnContext
{
	LvnWindowApi				windowapi;
	LvnWindowContext			windowContext;
	LvnGraphicsApi				graphicsapi;
	LvnGraphicsContext			graphicsContext;
	bool						logging;
	bool						vulkanValidationLayers;
	LvnLogger					coreLogger;
	LvnLogger					clientLogger;
	LvnPhysicalDevice*			physicalDevices;
	uint32_t					physicalDeviceCount;

	LvnPipelineSpecification	defaultPipelineSpecification;

	size_t						numMemoryAllocations;
};

#endif