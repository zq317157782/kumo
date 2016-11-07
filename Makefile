.PHONY: clean All

All:
	@echo "----------Building project:[ RayTracer - Debug ]----------"
	@"$(MAKE)" -f  "RayTracer.mk"
clean:
	@echo "----------Cleaning project:[ RayTracer - Debug ]----------"
	@"$(MAKE)" -f  "RayTracer.mk" clean
