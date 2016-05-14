CXXFLAGS ?= -std=c++11 -Wall -O2 -g

program = darts
sources = $(wildcard *.cc)
objects = $(sources:%.cc=%.o)
depends = $(sources:%.cc=%.d)

%.d: %.cc
	@$(CPP) $(CPPFLAGS) -c -MP -MM -MT "$@ $*.o" $< >$@

$(program): $(objects)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(program) $(objects) $(depends)

ifneq ($(MAKECMDGOALS),clean)
-include $(depends)
endif
