.section .data._binary_dark_saucisse_start, "aw"
.balign 4

.global _binary_dark_saucisse_start
.type _binary_dark_saucisse_start, @object
.size _binary_dark_saucisse_start, (_binary_dark_saucisse_start_end - _binary_dark_saucisse_start)
_binary_dark_saucisse_start:
	.incbin "C:/dev/psx/shadow2025/dark_saucisse.hit"

.local _binary_dark_saucisse_start_end
_binary_dark_saucisse_start_end:

.balign 4

.section .data._binary_dark_saucisse_start_size, "aw"
.balign 4

.global _binary_dark_saucisse_start_size
.type _binary_dark_saucisse_start_size, @object
.size _binary_dark_saucisse_start_size, 4
_binary_dark_saucisse_start_size:
	.int (_binary_dark_saucisse_start_end - _binary_dark_saucisse_start)
