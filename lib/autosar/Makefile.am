lib_LTLIBRARIES = libautosar.la

libautosar_la_LDFLAGS = @XENO_LIB_LDFLAGS@ -version-info 0:0:0

libautosar_la_SOURCES =		\
	os_types.h		\
	os_application.h	\
	os_isr.h		\
	os_isr.c		\
	os_scheduletable.h 	\
	os_spinlock.h		\
	os_event.h		\
	os_appmode.h		\
	os_task.c		\
	os_task.h		\
	os_alarm.c		\
	os_alarm.h		\
	os_os.c			\
	os_os.h			\
	app/app_define.h	\
	app/app_config.c	\
	app/app_config.h	\
	app/app_isr.c	        \
	app/app_isr.h	        \
	os_timer.c		\
	os_timer.h		\
	os_hook.h		\
	os_hook.c		\
	os_counter.c		\
	os_counter.h		\
	os_resource.h		\
	os_resource.c		\
        HW/Config.h             \
        Dio.h                   \
        Dio.c                   \
	init.c		

libautosar_la_CPPFLAGS =				\
	@XENO_USER_CFLAGS@				\
	-I$(top_srcdir)/include				\
	-I$(top_srcdir)/lib

EXTRA_DIST = testsuite

SPARSE = sparse

sparse:
	@for i in $(libautosar_la_SOURCES); do \
		$(SPARSE) $(CHECKFLAGS) $(srcdir)/$$i; \
	done
