
SHADERS = $(wildcard shaders/*.frag shaders/*.vert)
SPIRV_SHADERS = $(addsuffix .spv, $(SHADERS))

%.frag.spv: %.frag
	glslc $^ -o $@
%.vert.spv: %.vert
	glslc $^ -o $@

.PHONY: shader
shader: $(SPIRV_SHADERS)


.PHONY: debug
.PHONY: release
.PHONY: clean
.PHONY: MESSAGE

MESSAGE:
	@echo [Log] Building PlayVk Shaders

debug: MESSAGE shader
	@echo [Log] PlayVk Shaders have been built successfully
	
release: MESSAGE shader
	@echo [Log] PlayVk Shaders have been built successfully

clean:
	del $(subst /,\, $(SPIRV_SHADERS))
	@echo [Log] PlayVk Shader have been cleaned successfully
