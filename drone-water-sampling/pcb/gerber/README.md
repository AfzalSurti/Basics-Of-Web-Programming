# Gerber Files for Manufacturing

## Overview

This directory contains the Gerber files required for PCB manufacturing of the ESP32 Water Quality Monitoring System. These files follow the RS-274X standard and are compatible with most PCB manufacturers.

## File List

### Gerber Files
- `water_quality_pcb-F.Cu.gbr` - Top copper layer (front)
- `water_quality_pcb-B.Cu.gbr` - Bottom copper layer (back)
- `water_quality_pcb-F.Mask.gbr` - Top soldermask
- `water_quality_pcb-B.Mask.gbr` - Bottom soldermask
- `water_quality_pcb-F.SilkS.gbr` - Top silkscreen
- `water_quality_pcb-B.SilkS.gbr` - Bottom silkscreen
- `water_quality_pcb-Edge.Cuts.gbr` - Board outline
- `water_quality_pcb-F.Paste.gbr` - Top solder paste (for SMD assembly)
- `water_quality_pcb-B.Paste.gbr` - Bottom solder paste

### Drill Files
- `water_quality_pcb.drl` - NC Drill file (Excellon format)
- `water_quality_pcb-PTH.drl` - Plated through holes
- `water_quality_pcb-NPTH.drl` - Non-plated through holes

### Additional Files
- `water_quality_pcb.pos` - Pick and place file (centroid data)
- `water_quality_pcb.xml` - IPC-2581 format (alternative)

## Board Specifications

### Physical Dimensions
- **Board Size**: 100mm x 80mm
- **Thickness**: 1.6mm (standard)
- **Board Material**: FR-4 TG-130
- **Number of Layers**: 2 (Top + Bottom)

### Copper Specifications
- **Copper Weight**: 1 oz (35 μm)
- **Finished Copper**: 1 oz on both layers
- **Minimum Track Width**: 0.2mm (8 mil)
- **Minimum Clearance**: 0.2mm (8 mil)

### Drilling Specifications
- **Minimum Hole Size**: 0.3mm (12 mil)
- **Hole Size Tolerance**: ±0.05mm
- **Via Specifications**:
  - Pad diameter: 0.8mm
  - Drill diameter: 0.4mm
  - Annular ring: 0.2mm minimum

### Surface Finish Options
**Recommended**: ENIG (Electroless Nickel Immersion Gold)
- Best for: High-reliability applications
- Shelf life: 12 months
- Flat surface for fine-pitch components

**Alternative**: HASL (Lead-Free)
- Most economical option
- Shelf life: 6 months
- Suitable for through-hole and standard SMD

### Soldermask
- **Color**: Green (default) - Other colors available: Red, Blue, Black, White
- **Type**: LPI (Liquid Photo Imageable)
- **Minimum Mask Width**: 0.1mm
- **Mask Clearance**: 0.05mm from pads

### Silkscreen
- **Color**: White (default) on green soldermask
- **Minimum Line Width**: 0.15mm
- **Minimum Text Height**: 1.0mm
- **Location**: Component side (top)

### Special Requirements
- **Impedance Control**: Not required (standard stackup acceptable)
- **Controlled Depth Drilling**: Not required
- **Blind/Buried Vias**: None
- **Edge Plating**: Not required
- **Gold Fingers**: Not required
- **Countersink Holes**: Not required

## Manufacturing Notes

### Design Rules Check (DRC)
Before manufacturing, verify:
- ✓ Minimum track width: 0.2mm
- ✓ Minimum clearance: 0.2mm
- ✓ Minimum drill size: 0.3mm
- ✓ Annular ring: 0.2mm minimum
- ✓ Board outline closed
- ✓ No copper on edge cuts layer
- ✓ Soldermask clearance appropriate

### Quality Standards
- **IPC Class**: Class 2 (General Electronic Products)
- **Visual Inspection**: Per IPC-A-600
- **Electrical Testing**: Flying probe or fixtures
- **Impedance Testing**: Not required

### Panelization
- **Panel Size**: Single board or customer choice
- **Separation Method**: V-scoring or tab routing
- **Tooling Holes**: 3mm diameter at corners if panelized
- **Fiducials**: Global fiducials recommended for SMD assembly

### Testing Requirements
- **Electrical Test**: 100% continuity and isolation test
- **Method**: Flying probe (recommended for prototypes)
- **Test Voltage**: 250V DC for isolation test
- **Test Points**: Marked on silkscreen if needed

## Recommended Manufacturers

### Prototyping (Low Volume, Fast Turnaround)
1. **OSH Park** (USA)
   - Min Order: 3 boards
   - Turnaround: 12-14 days
   - Cost: ~$75 for 3 boards

2. **JLCPCB** (China)
   - Min Order: 5 boards
   - Turnaround: 2-5 days + shipping
   - Cost: ~$20 for 5 boards + shipping

3. **PCBWay** (China)
   - Min Order: 5 boards
   - Turnaround: 3-7 days + shipping
   - Cost: ~$25 for 5 boards + shipping

### Production (Higher Volume)
1. **Advanced Circuits** (USA)
   - IPC Class 2 or 3
   - Full testing available
   - Lead time: 5-10 days

2. **Eurocircuits** (Europe)
   - High quality production
   - Lead time: 5-12 days

## Assembly Services

If ordering SMD assembly, provide:
- Gerber files (this directory)
- BOM file (`../bom.csv`)
- Pick-and-place file (`.pos` file)
- Component orientation details

## File Verification

### Before Manufacturing
1. **Import Gerbers** into viewer (e.g., gerbv, KiCad GerbView)
2. **Check all layers** load correctly
3. **Verify board outline** is complete and correct size
4. **Check copper clearances** on both layers
5. **Verify drill file** holes align with pads
6. **Review silkscreen** for readability and clearances
7. **Check soldermask openings** on all pads

### Using Online Tools
- **PCBWay Online Viewer**: Upload Gerbers to visualize
- **OSH Park Preview**: See how board will look manufactured
- **EasyEDA Gerber Viewer**: Free online verification

## Ordering Instructions

### Standard Order
1. Create ZIP file containing all Gerber and drill files
2. Upload to manufacturer's website
3. Select specifications:
   - **Quantity**: 5-10 boards (prototype)
   - **Thickness**: 1.6mm
   - **Copper**: 1 oz
   - **Surface Finish**: ENIG or HASL
   - **Soldermask**: Green
   - **Silkscreen**: White
4. Review auto-generated preview
5. Proceed with order

### SMD Assembly Order
1. Upload Gerber files
2. Upload BOM file
3. Upload pick-and-place file
4. Select parts from manufacturer's library or provide parts
5. Review assembly quote
6. Order both PCB and assembly

## Support

For questions about Gerber files:
1. Verify files with Gerber viewer first
2. Check this README for specifications
3. Consult manufacturer's design guidelines
4. Refer to schematic.pdf for circuit details

## Revision History

- **v1.0** (2024-01-15): Initial release
  - 2-layer board design
  - 100mm x 80mm
  - All components routed
  - DRC clean

## License

These Gerber files are provided for manufacturing the Water Quality Monitoring System PCB as part of the complete drone water sampling project.
