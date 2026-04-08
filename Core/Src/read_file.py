from pathlib import Path
import time

SECTOR_SIZE = 512

# Match firmware
NUMBER_OF_SECTORS_TO_WRITE = 256
TOTAL_WRITE_LOOPS = 512
FILE_RESERVED_SECTORS = 1   # because card.START_SECTOR++ skips first sector of file

TOTAL_WRITTEN_SECTORS = NUMBER_OF_SECTORS_TO_WRITE * TOTAL_WRITE_LOOPS


def expected_value_for_sector(file_sector_index: int):
    """
    Returns expected byte value for the whole sector.
    None = don't check this sector.
    """
    if file_sector_index < FILE_RESERVED_SECTORS:
        return None

    written_start = FILE_RESERVED_SECTORS
    written_end = FILE_RESERVED_SECTORS + TOTAL_WRITTEN_SECTORS - 1

    if not (written_start <= file_sector_index <= written_end):
        return None

    pattern_sector = (file_sector_index - FILE_RESERVED_SECTORS) % NUMBER_OF_SECTORS_TO_WRITE
    return pattern_sector & 0xFF


def verify_bin_file(filepath: str):
    path = Path(filepath)

    if not path.exists():
        print(f"ERROR: File not found: {path}")
        return

    file_size = path.stat().st_size
    total_file_sectors = file_size // SECTOR_SIZE

    print(f"File              : {path}")
    print(f"File size         : {file_size / (1024 * 1024):.2f} MB")
    print(f"Total sectors     : {total_file_sectors}")
    print(f"Reserved sectors  : {FILE_RESERVED_SECTORS}")
    print(f"Written sectors   : {TOTAL_WRITTEN_SECTORS}")
    print(f"Pattern length    : {NUMBER_OF_SECTORS_TO_WRITE} sectors")
    print("Expected pattern  : sector values repeat as 0..255, 0..255, ...")
    print()

    checked_sectors = 0
    bad_sectors = 0
    first_errors = []

    start_time = time.time()
    print_every_sectors = 2048  # ~1 MB

    with open(path, "rb") as f:
        for sector_idx in range(total_file_sectors):
            sector = f.read(SECTOR_SIZE)
            if len(sector) != SECTOR_SIZE:
                print(f"Incomplete sector at {sector_idx}")
                break

            expected = expected_value_for_sector(sector_idx)
            if expected is None:
                continue

            checked_sectors += 1

            # entire sector should be same byte
            if any(b != expected for b in sector):
                bad_sectors += 1

                mismatches = []
                for byte_idx, actual in enumerate(sector):
                    if actual != expected:
                        mismatches.append((byte_idx, actual))
                        if len(mismatches) >= 8:
                            break

                if len(first_errors) < 20:
                    absolute_written_sector = sector_idx - FILE_RESERVED_SECTORS
                    expected_pattern_sector = absolute_written_sector % NUMBER_OF_SECTORS_TO_WRITE
                    first_errors.append({
                        "file_sector": sector_idx,
                        "pattern_sector": expected_pattern_sector,
                        "expected": expected,
                        "mismatches": mismatches,
                    })

            if sector_idx % print_every_sectors == 0:
                elapsed = time.time() - start_time
                processed_mb = (sector_idx * SECTOR_SIZE) / (1024 * 1024)
                speed = processed_mb / elapsed if elapsed > 0 else 0
                progress = (sector_idx / total_file_sectors) * 100 if total_file_sectors else 0
                remaining_mb = ((total_file_sectors - sector_idx) * SECTOR_SIZE) / (1024 * 1024)
                eta = remaining_mb / speed if speed > 0 else 0

                print(
                    f"\rProgress: {progress:6.2f}% | "
                    f"{processed_mb:8.1f} MB | "
                    f"{speed:6.1f} MB/s | "
                    f"ETA: {eta:5.1f} s",
                    end=""
                )

    print("\n\nVerification result")
    print("-------------------")
    print(f"Checked sectors : {checked_sectors}")
    print(f"Bad sectors     : {bad_sectors}")

    if bad_sectors == 0:
        print("PASS: All checked sectors match expected repeating 0..255 pattern.")
    else:
        print("FAIL: Mismatched sectors found.\n")
        print("First mismatches:")
        for err in first_errors:
            mismatch_text = ", ".join(
                [f"byte[{idx}]=0x{val:02X}" for idx, val in err["mismatches"]]
            )
            print(
                f"  File sector {err['file_sector']} "
                f"(pattern sector {err['pattern_sector']}): "
                f"expected 0x{err['expected']:02X}, found {mismatch_text}"
            )


if __name__ == "__main__":
    verify_bin_file("/home/ck/Desktop/flight_log.bin")