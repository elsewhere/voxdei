EXAMPLES = 3dtest basstest contest custloop dsptest fxtest livefx livespec \
	loadnget multi netradio plugins rectest speakers spectrum synth writewav

.PHONY: all clean $(EXAMPLES)

all: $(EXAMPLES)

clean:
	@$(foreach x,$(EXAMPLES),$(MAKE) -C $(x) clean;)

$(EXAMPLES):
	$(MAKE) -C $@
