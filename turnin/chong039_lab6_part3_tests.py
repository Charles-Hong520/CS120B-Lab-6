tests = [ {'description': 'PINA: 0x00 => PORTC: 0x00',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 5 } ],
    'expected': [('PORTC',0x00)],
    },
    ]