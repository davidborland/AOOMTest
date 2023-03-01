import vtk

inFileName = '1aua/1aua.obj'
outFilePrefix = '1aua/1aua_Clean'

reader = vtk.vtkOBJReader()
reader.SetFileName(inFileName)

clean = vtk.vtkCleanPolyData()
clean.SetInputConnection(reader.GetOutputPort())
clean.SetTolerance(0.0)
clean.SetAbsoluteTolerance(0.0)

mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(clean.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)

renderer = vtk.vtkRenderer()
renderer.AddViewProp(actor)

window = vtk.vtkRenderWindow()
window.AddRenderer(renderer)

print 'Reading ' + inFileName

window.Render()

exporter = vtk.vtkOBJExporter()
exporter.SetFilePrefix(outFilePrefix)
exporter.SetRenderWindow(window)
exporter.Write()

print 'Writing ' + outFilePrefix + '.obj'