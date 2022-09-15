.PHONY: all test_best_fit test_surcharge test_bf_limite test_manip_buffer clean

all: test_best_fit test_surcharge test_bf_limite test_manip_buffer

utils.mli: utils.ml
	ocamlc -i $< > $@

utils.cmi: utils.mli
	ocamlc -c $<

utils.cmo: utils.ml utils.cmi
	ocamlc -c $<

%.cmi: %.mli
	ocamlc -c $<

%.cmo: %.ml %.cmi utils.cmi utils.cmo
	ocamlc utils.cmo -c $<

%.o: %.c %.h
	gcc -pthread -c $<

test_best_fit: test_best_fit.ml repartiteur.cmi repartiteur.cmo utils.cmi utils.cmo
	ocamlc utils.cmo repartiteur.cmo $< -o $@
	@./$@

test_surcharge: test_surcharge.ml repartiteur.cmi repartiteur.cmo utils.cmi utils.cmo
	ocamlc utils.cmo repartiteur.cmo $< -o $@
	@./$@

test_bf_limite: test_bf_limite.ml repartiteur.cmi repartiteur.cmo utils.cmi utils.cmo
	ocamlc utils.cmo repartiteur.cmo $< -o $@
	@./$@

test_manip_buffer: test_manip_buffer.c manip_buffer.o utils.o
	gcc utils.o manip_buffer.o $< -o $@
	@./$@

signeur: signeur.c manip_buffer.o
	gcc -pthread manip_buffer.o $< -o $@

test_signeur: test_signeur.c manip_buffer.o
	gcc manip_buffer.o $< -o $@

clean:
	-@rm *.cmi *.cmo test_best_fit test_surcharge test_bf_limite
	-@rm *.o test_manip_buffer
