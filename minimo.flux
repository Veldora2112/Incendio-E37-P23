from(bucket: "mediciones")
	|> range(start: -40m)
	|> filter(fn: (r) => r.meansurement == "mediciones")
	|> filter(fn: (r) => r.equipo == "E37" and r.proyecto == "P23" and r.nodo == "demo")
	|> filter(fn: (r) => r._field == "resistencia_mq")
	|> filter(fn: (r) => r._value < 24)
	|> min()

