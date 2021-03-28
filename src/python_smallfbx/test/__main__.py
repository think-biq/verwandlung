import sys
import struct
import uuid
import smallfbx as sfbx


def make_uuid(int_id):
	byte_id = struct.pack('>q', int_id)
	return uuid.UUID(bytes=b'\x00'*8 + byte_id)


def main(args):
	if len(args) < 1:
		raise Exception('Expect FBX as input for test ...')

	filepath = args[1]

	d = sfbx.Document(filepath)
	print(f'FBX valid: {d.valid()}')

	if not d.valid():
		print(f'NOOOOOO!')
		sys.exit(2)

	print(f'FBX Version: {d.version()}')
	stacks = d.stacks()
	print(f'Animation Stacks: {stacks}')
	for s in stacks:
		print(f'{s.name()}[{s.full_name()}] @ {s.id()}')
		for l in s.layers():
			print(f'{l.name()}[{l.full_name()}] @ {l.id()}')
			for cn in l.curve_nodes():
				print(f'{cn.name()}[{cn.full_name()}] @ {cn.id()}: {cn.kind()} -> {cn.target().id()}')
				if cn.kind() == sfbx.AnimationKind.DeformWeight:
					print('Found deformer!')
				for c in cn.curves():
					print(f'{c.name()}[{c.full_name()}] @ {c.id()}: ')

if __name__ == '__main__':
	main(sys.argv)