TS_MODS=pdict_SUITE maps_SUITE
TS_OBJS=$(addsuffix .beam,$(TS_MODS))
OBJS=regressions.beam unit.beam matmul.beam fannkuchredux.beam mandelbrot.beam \
	ts_epi.beam $(TS_OBJS)

ERL=../bin/erl
ERLC=../bin/erlc
SLAVE=../bin/epiphany-unknown-elf/slave.smp.srec

.PHONY: fast test clean

test: $(OBJS)
	./run $(TS_MODS)

fast: $(OBJS)
	sudo -E LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) SLAVE_BINARY=$(SLAVE) \
	$(ERL) -noshell -eval \
	'case unit:test() of error -> halt(1); _ -> init:stop() end'

clean:
	rm -f $(OBJS)

%.beam: %.erl
	@echo -e "  ERLC\t"$<
	@$(ERLC) $<