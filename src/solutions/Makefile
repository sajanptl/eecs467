include ../common.mk

# flags for building the gtk library
CXXFLAGS = $(CXXFLAGS_STD) \
	$(CFLAGS_VX_GTK) \
	$(CFLAGS_IMAGESOURCE) \
	$(CFLAGS_MATH) \
	$(CFLAGS_COMMON) \
	$(CFLAGS_LCMTYPES) \
	$(CFLAGS_GLIB) \
	-O2
	
CFLAGS = $(CFLAGS_STD) \
	$(CFLAGS_VX_GTK) \
	$(CFLAGS_IMAGESOURCE) \
	$(CFLAGS_MATH) \
	$(CFLAGS_COMMON) \
	$(CFLAGS_LCMTYPES) \
	$(CFLAGS_GLIB) \
	-O2

LDFLAGS = $(LDFLAGS_STD) \
	$(LDFLAGS_VX_GTK) \
	$(LDFLAGS_IMAGESOURCE) \
	$(LDFLAGS_MATH) \
	$(LDFLAGS_COMMON) \
	$(LDFLAGS_LCMTYPES)

LIBDEPS = $(call libdeps, $(LDFLAGS))

LIB_EECS467 = $(LIB_PATH)/libeecs467.a
LIBEECS467_OBJS = vx_utils.o vx_gtk_window_base.o

KEY_PRODUCER = key_producer
KEY_PRODUCER_OBJS = key_producer.o
KEY_LISTENER = key_listener
KEY_LISTENER_OBJS = key_listener.o

ALL = $(LIB_EECS467) $(KEY_PRODUCER) $(KEY_LISTENER)

all: $(ALL)

$(KEY_LISTENER): $(KEY_LISTENER_OBJS) $(LIBDEPS)
	@echo "\t$@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

$(KEY_PRODUCER): $(KEY_PRODUCER_OBJS) $(LIBDEPS)
	@echo "\t$@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

$(LIB_EECS467): $(LIBEECS467_OBJS) $(LIBDEPS)
	@echo "\t$@"
	@ar rc $@ $^

clean:
	@rm -f *.o *~ *.a
	@rm -f $(ALL)
