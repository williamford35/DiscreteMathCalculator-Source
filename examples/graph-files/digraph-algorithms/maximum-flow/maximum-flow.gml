Creator "ogdf::GraphIO::writeGML"
graph
[
	directed	1
	node
	[
		id	0
		template	""
		label	"Source"
		type	"vertex"
		graphics
		[
			x	145.0000000000
			y	47.5000000000
			w	57.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	node
	[
		id	1
		template	""
		label	"A"
		type	"vertex"
		graphics
		[
			x	152.5000000000
			y	192.5000000000
			w	35.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	node
	[
		id	2
		template	""
		label	"B"
		type	"vertex"
		graphics
		[
			x	107.5000000000
			y	107.5000000000
			w	35.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	node
	[
		id	3
		template	""
		label	"C"
		type	"vertex"
		graphics
		[
			x	92.5000000000
			y	252.5000000000
			w	35.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	node
	[
		id	4
		template	""
		label	"D"
		type	"vertex"
		graphics
		[
			x	47.5000000000
			y	192.5000000000
			w	35.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	node
	[
		id	5
		template	""
		label	"Sink"
		type	"vertex"
		graphics
		[
			x	70.0000000000
			y	312.5000000000
			w	35.0000000000
			h	35.0000000000
			fill	"#FFD700"
			fillbg	"#000000"
			outline	"#000000"
			pattern	"Solid"
			stipple	"Solid"
			lineWidth	1.0000000000
			type	"Rect"
		]
	]
	edge
	[
		source	0
		target	1
		generalization	0
		label "8"
		weight	1.0000000000
		intWeight	8
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 145.0000000000 y 47.5000000000 ]
				point [ x 182.5000000000 y 107.5000000000 ]
				point [ x 182.5000000000 y 150.0000000000 ]
				point [ x 152.5000000000 y 192.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	0
		target	2
		generalization	0
		label "7"
		weight	1.0000000000
		intWeight	7
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			fill "#000000"
		]
	]
	edge
	[
		source	1
		target	2
		generalization	0
		label "5"
		weight	1.0000000000
		intWeight	5
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 152.5000000000 y 192.5000000000 ]
				point [ x 152.5000000000 y 150.0000000000 ]
				point [ x 107.5000000000 y 107.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	2
		target	1
		generalization	0
		label "3"
		weight	1.0000000000
		intWeight	3
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 107.5000000000 y 107.5000000000 ]
				point [ x 122.5000000000 y 150.0000000000 ]
				point [ x 152.5000000000 y 192.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	1
		target	3
		generalization	0
		label "2"
		weight	1.0000000000
		intWeight	2
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			fill "#000000"
		]
	]
	edge
	[
		source	3
		target	2
		generalization	0
		label "1"
		weight	1.0000000000
		intWeight	1
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 92.5000000000 y 252.5000000000 ]
				point [ x 92.5000000000 y 192.5000000000 ]
				point [ x 92.5000000000 y 150.0000000000 ]
				point [ x 107.5000000000 y 107.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	2
		target	4
		generalization	0
		label "6"
		weight	1.0000000000
		intWeight	6
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 107.5000000000 y 107.5000000000 ]
				point [ x 47.5000000000 y 150.0000000000 ]
				point [ x 47.5000000000 y 192.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	4
		target	3
		generalization	0
		label "2"
		weight	1.0000000000
		intWeight	2
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			fill "#000000"
		]
	]
	edge
	[
		source	4
		target	5
		generalization	0
		label "5"
		weight	1.0000000000
		intWeight	5
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			Line [
				point [ x 47.5000000000 y 192.5000000000 ]
				point [ x 47.5000000000 y 252.5000000000 ]
				point [ x 70.0000000000 y 312.5000000000 ]
			]
			fill "#000000"
		]
	]
	edge
	[
		source	3
		target	5
		generalization	0
		label "6"
		weight	1.0000000000
		intWeight	6
		graphics
		[
			type	"line"
			arrow	"last"
			stipple	"Solid"
			lineWidth	1.0000000000
			fill "#000000"
		]
	]
]
# digraphw
