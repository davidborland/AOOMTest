import vtk

inFileName = 'sfh1_PC_PI/balls.obj'

reader = vtk.vtkOBJReader()
reader.SetFileName(inFileName)

mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(reader.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)

renderer = vtk.vtkRenderer()
renderer.AddViewProp(actor)

window = vtk.vtkRenderWindow()
window.AddRenderer(renderer)

interactor = vtk.vtkRenderWindowInteractor()
interactor.SetRenderWindow(window)

interactor.Initialize()
interactor.Start()