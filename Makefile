.PHONY: clean All

All:
	@echo "----------Building project:[ IDE_test0 - Debug ]----------"
	@cd "IDE_test0" && "$(MAKE)" -f  "IDE_test0.mk"
clean:
	@echo "----------Cleaning project:[ IDE_test0 - Debug ]----------"
	@cd "IDE_test0" && "$(MAKE)" -f  "IDE_test0.mk" clean
